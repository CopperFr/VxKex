#include "buildcfg.h"
#include "kxbasep.h"

KXBASEAPI BOOL WINAPI GetProcessInformation(
	IN	HANDLE						ProcessHandle,
	IN	PROCESS_INFORMATION_CLASS	ProcessInformationClass,
	OUT	PVOID						ProcessInformation,
	IN	ULONG						ProcessInformationSize)
{
	// TODO
	BaseSetLastNTError(STATUS_NOT_IMPLEMENTED);
	return FALSE;
}

KXBASEAPI BOOL WINAPI SetProcessInformation(
	IN	HANDLE						ProcessHandle,
	IN	PROCESS_INFORMATION_CLASS	ProcessInformationClass,
	IN	PVOID						ProcessInformation,
	IN	ULONG						ProcessInformationSize)
{
	// TODO
	BaseSetLastNTError(STATUS_NOT_IMPLEMENTED);
	return FALSE;
}

KXBASEAPI BOOL WINAPI SetProcessDefaultCpuSets(
	IN	HANDLE	ProcessHandle,
	IN	PULONG	CpuSetIds,
	IN	ULONG	NumberOfCpuSetIds)
{
	if (CpuSetIds == NULL) {
		if (NumberOfCpuSetIds != 0) {
			BaseSetLastNTError(STATUS_INVALID_PARAMETER);
			return FALSE;
		}
	}

	return TRUE;
}

KXBASEAPI BOOL WINAPI SetProcessDefaultCpuSetMasks(
	IN	HANDLE			ProcessHandle,
	IN	PGROUP_AFFINITY	CpuSetMasks,
	IN	ULONG			NumberOfCpuSetMasks)
{
	if (CpuSetMasks == NULL) {
		if (NumberOfCpuSetMasks != 0) {
			BaseSetLastNTError(STATUS_INVALID_PARAMETER);
			return FALSE;
		}
	}

	return TRUE;
}

KXBASEAPI BOOL WINAPI GetProcessDefaultCpuSets(
	IN	HANDLE	ProcessHandle,
	OUT	PULONG	CpuSetIds,
	IN	ULONG	CpuSetIdArraySize,
	OUT	PULONG	ReturnCount)
{
	*ReturnCount = 0;

	if (CpuSetIds == NULL) {
		if (CpuSetIdArraySize != 0) {
			BaseSetLastNTError(STATUS_INVALID_PARAMETER);
			return FALSE;
		}
	}

	return TRUE;
}

KXBASEAPI BOOL WINAPI GetProcessDefaultCpuSetMasks(
	IN	HANDLE			ProcessHandle,
	OUT	PGROUP_AFFINITY	CpuSetMasks,
	IN	ULONG			CpuSetMaskArraySize,
	OUT	PULONG			ReturnCount)
{
	*ReturnCount = 0;

	if (CpuSetMasks == NULL) {
		if (CpuSetMaskArraySize != 0) {
			BaseSetLastNTError(STATUS_INVALID_PARAMETER);
			return FALSE;
		}
	}

	return TRUE;
}

KXBASEAPI BOOL WINAPI SetProcessMitigationPolicy(
	IN	PROCESS_MITIGATION_POLICY	MitigationPolicy,
	IN	PVOID						Buffer,
	IN	SIZE_T						BufferCb)
{
	//
	// Note that Windows 7 has SetProcessDEPPolicy but it doesn't do anything
	// for x64.
	//

	if (KexRtlCurrentProcessBitness() == 32 && MitigationPolicy == ProcessDEPPolicy) {
		PPROCESS_MITIGATION_DEP_POLICY DepPolicy;

		if (BufferCb != sizeof(PROCESS_MITIGATION_DEP_POLICY)) {
			BaseSetLastNTError(STATUS_INVALID_BUFFER_SIZE);
			return FALSE;
		}

		DepPolicy = (PPROCESS_MITIGATION_DEP_POLICY) Buffer;

		if (DepPolicy->Flags.ReservedFlags) {
			BaseSetLastNTError(STATUS_INVALID_PARAMETER);
			return FALSE;
		}

		return SetProcessDEPPolicy(DepPolicy->Flags.AsUlong);
	} else {
		KexLogWarningEvent(
			L"SetProcessMitigationPolicy called with unsupported MitigationPolicy value %d",
			MitigationPolicy);

		// Fall through and pretend we succeeded.
	}

	return TRUE;
}

KXBASEAPI BOOL WINAPI GetProcessMitigationPolicy(
	IN	HANDLE						ProcessHandle,
	IN	PROCESS_MITIGATION_POLICY	MitigationPolicy,
	OUT	PVOID						Buffer,
	IN	SIZE_T						BufferCb)
{
	if (MitigationPolicy == ProcessMitigationOptionsMask) {
		PULONGLONG Mask;

		//
		// Buffer is a pointer to either one or two ULONGLONGs.
		// The first one contains PROCESS_CREATION_MITIGATION_POLICY_*.
		// The second one if present contains PROCESS_CREATION_MITIGATION_POLICY2_*
		//

		if (BufferCb != 8 && BufferCb != 16) {
			BaseSetLastNTError(STATUS_INVALID_PARAMETER);
			return FALSE;
		}

		Mask = (PULONGLONG) Buffer;

		Mask[0] = PROCESS_CREATION_MITIGATION_POLICY_VALID_MASK;

		if (BufferCb > 8) {
			Mask[1] = 0;
		}

		return TRUE;
	} else if (MitigationPolicy == ProcessDEPPolicy) {
		BOOLEAN Success;
		PPROCESS_MITIGATION_DEP_POLICY DepPolicy;
		BOOL Permanent;

		if (BufferCb != sizeof(PROCESS_MITIGATION_DEP_POLICY)) {
			BaseSetLastNTError(STATUS_INVALID_BUFFER_SIZE);
			return FALSE;
		}

		DepPolicy = (PPROCESS_MITIGATION_DEP_POLICY) Buffer;

		Success = GetProcessDEPPolicy(
			NtCurrentProcess(),
			&DepPolicy->Flags.AsUlong,
			&Permanent);

		DepPolicy->Permanent = Permanent;
		return Success;
	} else if (MitigationPolicy == ProcessSystemCallDisablePolicy) {
		PPROCESS_MITIGATION_SYSTEM_CALL_DISABLE_POLICY SystemCallDisablePolicy;

		if (BufferCb != sizeof(PROCESS_MITIGATION_SYSTEM_CALL_DISABLE_POLICY)) {
			BaseSetLastNTError(STATUS_INVALID_BUFFER_SIZE);
			return FALSE;
		}

		SystemCallDisablePolicy = (PPROCESS_MITIGATION_SYSTEM_CALL_DISABLE_POLICY) Buffer;

		SystemCallDisablePolicy->DisallowWin32kSystemCalls = 0;
		SystemCallDisablePolicy->AuditDisallowWin32kSystemCalls = 0;
		SystemCallDisablePolicy->DisallowFsctlSystemCalls = 0;
		SystemCallDisablePolicy->AuditDisallowFsctlSystemCalls = 0;

		return TRUE;
	} else if (MitigationPolicy == ProcessPayloadRestrictionPolicy) {
		PPROCESS_MITIGATION_PAYLOAD_RESTRICTION_POLICY PayloadRestrictionPolicy;
		
		if (BufferCb != sizeof(PROCESS_MITIGATION_PAYLOAD_RESTRICTION_POLICY)) {
			BaseSetLastNTError(STATUS_INVALID_BUFFER_SIZE);
			return FALSE;
		}
		
		PayloadRestrictionPolicy = (PPROCESS_MITIGATION_PAYLOAD_RESTRICTION_POLICY) Buffer;
		
		PayloadRestrictionPolicy->Flags.AsUlong = 0;
		return TRUE;
	} else {
		KexLogWarningEvent(
			L"GetProcessMitigationPolicy called with unsupported MitigationPolicy value %d",
			MitigationPolicy);
	}

	BaseSetLastNTError(STATUS_NOT_SUPPORTED);
	return FALSE;
}

KXBASEAPI BOOL WINAPI Ext_IsProcessInJob(
	IN	HANDLE	ProcessHandle,
	IN	HANDLE	JobHandle,
	OUT	PBOOL	IsInJob)
{
	//
	// APPSPECIFICHACK: Make Chromium non-official builds not meddle with the IAT.
	// See sandbox\policy\win\sandbox_win.cc SandboxWin::InitBrokerServices.
	// This function should only be called in one place.
	//

	if ((KexData->Flags & KEXDATA_FLAG_CHROMIUM) &&
		AshModuleBaseNameIs(ReturnAddress(), L"chrome.dll")) {

		ASSERT (ProcessHandle == NtCurrentProcess());
		ASSERT (JobHandle == NULL);
		ASSERT (IsInJob != NULL);

		KexLogDebugEvent(L"Returning fake IsProcessInJob return value for Chrome compatibility");

		if (ProcessHandle == NtCurrentProcess() &&
			JobHandle == NULL &&
			IsInJob != NULL) {

			*IsInJob = TRUE;
			return TRUE;
		}
	}

	return IsProcessInJob(ProcessHandle, JobHandle, IsInJob);
}

KXBASEAPI BOOL WINAPI Ext_UpdateProcThreadAttribute(
	IN OUT	PPROC_THREAD_ATTRIBUTE_LIST	AttributeList,
	IN		ULONG						Flags,
	IN		ULONG_PTR					Attribute,
	IN		PVOID						Value,
	IN		SIZE_T						Size,
	OUT		PVOID						PreviousValue OPTIONAL,
	OUT		PSIZE_T						ReturnSize OPTIONAL)
{
	ULONG MitigationPolicy;
	
	// Lastests Go Lang removed some changes for Window 7 let try to add it here...
	if ((Attribute == PROC_THREAD_ATTRIBUTE_HANDLE_LIST)) {
		SIZE_T I, J = 0;
#ifdef _M_X64
		for (I = 0; I < Size / sizeof(ULONG64); I++) {
			ULONG64 Handle = ((PULONG64)Value)[I];
			if ((Handle & 0x10000003) != 3) {
				((PULONG64)Value)[J++] = ((PULONG64)Value)[I];
			}
		}
		Size = J * sizeof(ULONG64);
#else
		for (I = 0; I < Size / sizeof(ULONG32); I++) {
			ULONG32 Handle = ((PULONG32)Value)[I];
			if ((Handle & 0x10000003) != 3) {
				((PULONG32)Value)[J++] = ((PULONG32)Value)[I];
			}
		}
		Size = J * sizeof(PULONG32);
#endif
		if (Size == 0) {
			// 0 handle left so don't call command it would fail
			KexLogInformationEvent(L"Removed all handles for PROC_THREAD_ATTRIBUTE_HANDLE_LIST");
			return TRUE;
		}
		else if (I != J) {
			KexLogInformationEvent(L"Removed some handles for PROC_THREAD_ATTRIBUTE_HANDLE_LIST");
		}
	}
	else if (Attribute == PROC_THREAD_ATTRIBUTE_PARENT_PROCESS) {
		KexLogInformationEvent(L"UpdateProcThreadAttribute(PROC_THREAD_ATTRIBUTE_PARENT_PROCESS)");
	}
	else if (Attribute == PROC_THREAD_ATTRIBUTE_MITIGATION_POLICY) {
		KexLogInformationEvent(L"UpdateProcThreadAttribute(PROC_THREAD_ATTRIBUTE_MITIGATION_POLICY)");
		if (Size != sizeof(ULONG)) {
			//ASSERT(Size == sizeof(ULONGLONG) || Size == 2 * sizeof(ULONGLONG));
			Size = sizeof(ULONG);
			MitigationPolicy = (*(PULONG)Value) & PROCESS_CREATION_MITIGATION_POLICY_VALID_MASK;
			Value = &MitigationPolicy;
		}
	}

	if (!UpdateProcThreadAttribute(
		AttributeList,
		Flags,
		Attribute,
		Value,
		Size,
		PreviousValue,
		ReturnSize))
	{
		ULONG LastError;

		LastError = GetLastError();

		if (LastError == ERROR_NOT_SUPPORTED) {
			SetLastError(ERROR_SUCCESS);
		}
		else {						
			KexLogWarningEvent(
				L"UpdateProcThreadAttribute(%d)\r\n\r\n"
				L"Last error code: %d",
				Attribute, LastError);

			// (LastError == ERROR_BAD_LENGTH) IF SIZE NOT CORRECTED FOR 32 BITS
			//PROC_THREAD_ATTRIBUTE_REPLACE_VALUE = 1U;
			//PROC_THREAD_ATTRIBUTE_PARENT_PROCESS = 131072U;
			//PROC_THREAD_ATTRIBUTE_HANDLE_LIST = 131074U;
			//PROC_THREAD_ATTRIBUTE_GROUP_AFFINITY = 196611U;
			//PROC_THREAD_ATTRIBUTE_PREFERRED_NODE = 131076U;
			//PROC_THREAD_ATTRIBUTE_IDEAL_PROCESSOR = 196613U;
			//PROC_THREAD_ATTRIBUTE_UMS_THREAD = 196614U;
			//PROC_THREAD_ATTRIBUTE_MITIGATION_POLICY = 131079U;
			//PROC_THREAD_ATTRIBUTE_SECURITY_CAPABILITIES = 131081U;
			//PROC_THREAD_ATTRIBUTE_PROTECTION_LEVEL = 131083U;
			//PROC_THREAD_ATTRIBUTE_PSEUDOCONSOLE = 131094U;
			//PROC_THREAD_ATTRIBUTE_MACHINE_TYPE = 131097U;
			//PROC_THREAD_ATTRIBUTE_ENABLE_OPTIONAL_XSTATE_FEATURES = 196635U;
			//PROC_THREAD_ATTRIBUTE_WIN32K_FILTER = 131088U;
			//PROC_THREAD_ATTRIBUTE_JOB_LIST = 131085U;
			//PROC_THREAD_ATTRIBUTE_CHILD_PROCESS_POLICY = 131086U;
			//PROC_THREAD_ATTRIBUTE_ALL_APPLICATION_PACKAGES_POLICY = 131087U;
			//PROC_THREAD_ATTRIBUTE_DESKTOP_APP_POLICY = 131090U;
			//PROC_THREAD_ATTRIBUTE_MITIGATION_AUDIT_POLICY = 131096U;
			//PROC_THREAD_ATTRIBUTE_COMPONENT_FILTER = 131098U;
			//PROC_THREAD_ATTRIBUTE_NUMBER = 65535U;
			//PROC_THREAD_ATTRIBUTE_THREAD = 65536U;
			//PROC_THREAD_ATTRIBUTE_INPUT = 131072U;
			//PROC_THREAD_ATTRIBUTE_ADDITIVE = 262144U;

			return FALSE;
		}
	}
	return TRUE;
}