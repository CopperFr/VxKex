#include "buildcfg.h"
#include "kxbasep.h"

KXBASEAPI BOOL WINAPI Ext_SetHandleInformation(
	IN HANDLE	hObject,
	IN DWORD	dwMask,
	IN DWORD	dwFlags)
{
	if ((((ULONG_PTR)(hObject) & 0x3) == 0x3 && GetFileType(hObject) == FILE_TYPE_CHAR)) {		
		dwMask &= ~HANDLE_FLAG_INHERIT;
		if (dwMask == 0) {
			return -1;
		}
		dwFlags &= ~HANDLE_FLAG_INHERIT;
	}
	return SetHandleInformation(hObject, dwMask, dwFlags);
}