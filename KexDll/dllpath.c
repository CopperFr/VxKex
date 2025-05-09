///////////////////////////////////////////////////////////////////////////////
//
// Module Name:
//
//     dllpath.c
//
// Abstract:
//
//     Contains routines for adding the Kex32/64 directory to the loader
//     search path, so that the process can load our extended DLLs. The
//     loader search path is a UNICODE_STRING structure, contained within
//     Peb->ProcessParameters->DllPath. This string contains semicolon-
//     separated Win32 paths, which is the same format as used in the %Path%
//     environment variable.
//
//     This is a relatively complex problem with a number of constraints.
//
//       1. We need to add KexDir\Kex32 or KexDir\Kex64 to the loader search
//          path, based on whether this is a 32 bit or 64 bit process.
//
//       2. The old length of the search path must be maintained, as the loader
//          caches the length value. Therefore, the search path cannot be
//          extended or contracted.
//
//       3. We must not disturb the presence or the order of existing search
//          path entries at all.
//
//     Fortunately, a number of factors make this possible in most cases:
//
//       1. The loader does not cache the actual data inside DllPath.Buffer,
//          so we can modify this buffer and influence the loader.
//
//   2 & 3. We can contract the length of the search path by removing
//          duplicate path entries. For example, %WinDir%\system32,
//          %WinDir%\system, and %WinDir% are all almost always duplicated
//          (since the system places them at the beginning, and then pastes
//          the %Path% environment variable after them, which contains
//          identical entries).
//
//          Sometimes, it is possible to collapse the length by removing
//          duplicate semicolons; however, this is much less likely to produce
//          a useful result and the current code implementation does not use
//          this method.
//
//          We can also pad the length of the search path out to the original
//          length by simply filling unused space with semicolons.
//
//   Keep in mind that the loader search path described above is only applicable
//   to process initialization AND when LdrLoadDll is called with NULL as the
//   first parameter (the DllPath parameter).
//
//   When DLLs are loaded using Kernel32/KernelBase functions such as
//   LoadLibrary, then the BASE dlls supply their own DllPath parameter to
//   LdrLoadDll which is passed down throughout the entire DLL load sequence
//   including the resolution of static imports. Therefore, the default loader
//   DllPath which is accessible through the PEB is not applicable after the
//   end of process initialization (except for the unlikely event of someone
//   calling LdrLoadDll with NULL as the first parameter).
//
//   In order to ensure that DLLs subsequently loaded have their static imports
//   correctly written and resolved, we will also need to modify the %PATH%
//   environment variable (which is relatively easy compared with modifying the
//   default loader DllPath).
//
// Author:
//
//     vxiiduu (30-Oct-2022)
//
// Revision History:
//
//     vxiiduu              30-Oct-2022  Initial creation.
//     vxiiduu              31-Oct-2022  Fix bugs and finalize implementation.
//     vxiiduu              18-Mar-2024  Fix a bug where forward slashes in the
//                                       DllPath could impede the ability of
//                                       KexpShrinkDllPathLength to do its job.
//     vxiiduu              05-Apr-2024  Correct more bugs.
//
///////////////////////////////////////////////////////////////////////////////

#include "buildcfg.h"
#include "kexdllp.h"

STATIC INLINE NTSTATUS KexpShrinkDllPathLength(
	IN	PUNICODE_STRING	DllPath,
	IN	USHORT			TargetLength);

STATIC INLINE NTSTATUS KexpPadDllPathToOriginalLength(
	IN	PUNICODE_STRING	DllPath,
	IN	USHORT			OriginalLength);

STATIC INLINE VOID KexpNormalizeDllPathBackslashes(
	IN OUT	PUNICODE_STRING	DllPath);

NTSTATUS KexpAddKex3264ToDllPath(
	VOID)
{
	NTSTATUS Status;
	PUNICODE_STRING DllPath;
	UNICODE_STRING NewDllPath;
	USHORT DllPathOriginalLength;
	ULONG Index;
	
	ASSERT (VALID_UNICODE_STRING(&KexData->Kex3264DirPath));
	ASSERT (KexData->Kex3264DirPath.Length != 0);

	DllPath = &NtCurrentPeb()->ProcessParameters->DllPath;
	DllPathOriginalLength = DllPath->Length;

	KexLogInformationEvent(
		L"Shrinking default loader DLL path\r\n\r\n"
		L"The original DLL path is: \"%wZ\"",
		DllPath);

	//
	// Convert all forward slashes in the DllPath to backslashes.
	// At least one real world case has been observed where a user's computer
	// had the Path environment variable contain forward slashes instead of
	// backslashes for some reason.
	//
	// Without normalizing the path separators, KexpShrinkDllPathLength would
	// fail. This would cause VxKex to not work.
	//

	KexpNormalizeDllPathBackslashes(DllPath);

	//
	// Call a helper function to shrink DllPath by *at least* the length
	// of our prepend string.
	//

	Status = KexpShrinkDllPathLength(DllPath, DllPath->Length - KexData->Kex3264DirPath.Length);
	ASSERT (NT_SUCCESS(Status));
	ASSERT (DllPath->Length < DllPathOriginalLength);
	ASSERT (DllPath->Length <= DllPathOriginalLength - KexData->Kex3264DirPath.Length);

	if (!NT_SUCCESS(Status)) {
		return Status;
	}

	//
	// Create a temporary buffer to hold the new DllPath.
	//

	NewDllPath.Length = 0;
	NewDllPath.MaximumLength = DllPath->Length + KexData->Kex3264DirPath.Length;
	NewDllPath.Buffer = StackAlloc(WCHAR, KexRtlUnicodeStringBufferCch(&NewDllPath));

	//
	// Build the new DllPath.
	//

	Status = RtlAppendUnicodeStringToString(&NewDllPath, &KexData->Kex3264DirPath);
	ASSERT (NT_SUCCESS(Status));

	if (!NT_SUCCESS(Status)) {
		return Status;
	}

	Status = RtlAppendUnicodeStringToString(&NewDllPath, DllPath);
	ASSERT (NT_SUCCESS(Status));

	if (!NT_SUCCESS(Status)) {
		return Status;
	}

	//
	// Copy new DllPath to old DllPath and finally pad to original length.
	//

	RtlCopyUnicodeString(DllPath, &NewDllPath);
	
	Status = KexpPadDllPathToOriginalLength(DllPath, DllPathOriginalLength);
	//ASSERT (NT_SUCCESS(Status));

	//
	// Remove any embedded nulls in the path. Sometimes the system or another
	// application can add them and it causes a problem with logging.
	//

	for (Index = 0; Index < KexRtlUnicodeStringCch(DllPath); ++Index) {
		if (DllPath->Buffer[Index] == '\0') {
			DllPath->Buffer[Index] = ';';
		}
	}

	return STATUS_SUCCESS;
}

//
// Try to shrink the DllPath length to equal or less than "Length" by
// removing duplicate entries.
//
STATIC INLINE NTSTATUS KexpShrinkDllPathLength(
	IN	PUNICODE_STRING	DllPath,
	IN	USHORT			TargetLength)
{
	NTSTATUS Status;
	UNICODE_STRING DllPathAfterCurrentEntry;
	UNICODE_STRING CurrentPathEntry;
	UNICODE_STRING DuplicatePathEntry;
	UNICODE_STRING Semicolon;

	RtlInitConstantUnicodeString(&Semicolon, L";");
	DllPathAfterCurrentEntry = *DllPath;

	until (DllPath->Length <= TargetLength) {

		//
		// Fetch a path entry
		//

		Status = RtlFindCharInUnicodeString(
			0,
			&DllPathAfterCurrentEntry,
			&Semicolon,
			&CurrentPathEntry.Length);

		if (!NT_SUCCESS(Status)) {
			KexLogErrorEvent(
				L"RtlFindCharInUnicodeString returned an error\r\n\r\n"
				L"NTSTATUS error code: %s",
				KexRtlNtStatusToString(Status));
			return Status;
		}

		CurrentPathEntry.Length -= sizeof(WCHAR); // it includes the semicolon - get rid of it
		CurrentPathEntry.Buffer = DllPathAfterCurrentEntry.Buffer;
		CurrentPathEntry.MaximumLength = CurrentPathEntry.Length;

		KexLogInformationEvent(
			L"Searching duplicate for %wZ\r\n\r\n",
			&CurrentPathEntry
		);

		//
		// Look for one or more duplicate entries later in the path.
		//

		while (TRUE) {
			UNICODE_STRING AfterDuplicate;
			UNICODE_STRING StringToSearchFor;

			RtlInitEmptyUnicodeStringFromTeb(&StringToSearchFor);
			RtlAppendUnicodeStringToString(&StringToSearchFor, &Semicolon);
			RtlAppendUnicodeStringToString(&StringToSearchFor, &CurrentPathEntry);
			RtlAppendUnicodeStringToString(&StringToSearchFor, &Semicolon);

			DuplicatePathEntry.Buffer = KexRtlFindUnicodeSubstring(
				&DllPathAfterCurrentEntry,
				&StringToSearchFor,
				TRUE);

			if (!DuplicatePathEntry.Buffer) {
				StringToSearchFor.Length -= 1;
				DuplicatePathEntry.Buffer = KexRtlFindUnicodeSubstring(
					&DllPathAfterCurrentEntry,
					&StringToSearchFor,
					TRUE);

				if (DuplicatePathEntry.Buffer + CurrentPathEntry.Length / 2 + 1 == DllPath->Buffer + DllPath->Length / 2) {
					DllPath->Length -= CurrentPathEntry.Length + 1;
					KexLogInformationEvent(
						L"Duplicate found at end of path for %wZ\r\n\r\n",
						&CurrentPathEntry
					);
				}				
				break;
			}

			KexLogInformationEvent(
				L"Duplicate found for %wZ\r\n\r\n",
				&CurrentPathEntry
			);

			DuplicatePathEntry.Buffer += 1;
			DuplicatePathEntry.Length = CurrentPathEntry.Length;
			DuplicatePathEntry.MaximumLength = (USHORT)
				((KexRtlEndOfUnicodeString(DllPath) - DuplicatePathEntry.Buffer) * sizeof(WCHAR));

			//
			// We need to cut this path entry out of the original DllPath and update the
			// length field accordingly. To do this, we will copy all characters from
			// the end of the duplicate path entry over top of the beginning of the
			// duplicate entry.
			//

			AfterDuplicate.Buffer = KexRtlEndOfUnicodeString(&DuplicatePathEntry);
			AfterDuplicate.Length = (USHORT) ((KexRtlEndOfUnicodeString(DllPath) - AfterDuplicate.Buffer) * sizeof(WCHAR));
			AfterDuplicate.MaximumLength = AfterDuplicate.Length;

			// skip over the next semicolon
			ASSERT (AfterDuplicate.Length != 0);
			ASSERT (AfterDuplicate.Buffer[0] == ';');
			KexRtlAdvanceUnicodeString(&AfterDuplicate, sizeof(WCHAR));

			DllPath->Length -= DuplicatePathEntry.Length;
			RtlCopyUnicodeString(&DuplicatePathEntry, &AfterDuplicate);
		}

		KexRtlAdvanceUnicodeString(&DllPathAfterCurrentEntry, CurrentPathEntry.Length + sizeof(WCHAR));
	}

	return STATUS_SUCCESS;
}

STATIC INLINE NTSTATUS KexpPadDllPathToOriginalLength(
	IN	PUNICODE_STRING	DllPath,
	IN	USHORT			OriginalLength)
{
	PWCHAR Pointer;

	if (DllPath->Length > OriginalLength) {
		return STATUS_INTERNAL_ERROR;
	}

	//
	// Add semicolons to the end until DllPath reaches the correct length.
	//

	Pointer = KexRtlEndOfUnicodeString(DllPath);
	DllPath->Length = OriginalLength;

	while (Pointer < KexRtlEndOfUnicodeString(DllPath)) {
		*Pointer++ = ';';
	}

	return STATUS_SUCCESS;
}

//
// Convert all slashes in the specified DllPath to backslashes.
//
STATIC INLINE VOID KexpNormalizeDllPathBackslashes(
	IN OUT	PUNICODE_STRING	DllPath)
{
	ULONG Index;
	ULONG DllPathCch;

	ASSUME (VALID_UNICODE_STRING(DllPath));

	DllPathCch = KexRtlUnicodeStringCch(DllPath);
	ASSUME (DllPathCch > 0);

	for (Index = 0; Index < DllPathCch; ++Index) {
		if (DllPath->Buffer[Index] == '/') {
			DllPath->Buffer[Index] = '\\';
			// Change [:;]/[c-z]/ => c:\ 
			if ((Index > 0) && (DllPath->Buffer[Index - 1] == ':' || DllPath->Buffer[Index - 1] == ';') && (Index < DllPathCch - 2) && DllPath->Buffer[Index + 2] == '/' && (DllPath->Buffer[Index + 1] >= 'c') && (DllPath->Buffer[Index + 1] <= 'z')) {
				DllPath->Buffer[Index - 1] = ';';
				DllPath->Buffer[Index] = DllPath->Buffer[Index + 1];
				DllPath->Buffer[Index + 1] = ':';
				DllPath->Buffer[Index + 2] = '\\';
			}
		}
	}
}