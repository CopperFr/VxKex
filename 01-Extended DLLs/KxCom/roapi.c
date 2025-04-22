#include "buildcfg.h"
#include "kxcomp.h"

UINT ErrorReportingFlags = 0;

KXCOMAPI HRESULT WINAPI RoGetAgileReference(
	IN	ULONG		Options,
	IN	REFIID		RefIID,
	IN	IUnknown	*pUnknown,
	OUT	IUnknown	**AgileReference)
{
	KexLogWarningEvent(L"Unimplemented stub function RoGetAgileReference was called");
	return E_NOTIMPL;
}

KXCOMAPI HRESULT WINAPI RoSetErrorReportingFlags(
	IN	UINT32 Flags)
{
	ErrorReportingFlags = Flags;
	return S_OK;
}

KXCOMAPI HRESULT WINAPI RoGetErrorReportingFlags(
	OUT	UINT32	*pFlags)
{
	if (pFlags == NULL) {
		return E_POINTER;
	} else {
		*pFlags = ErrorReportingFlags;
		return S_OK;
	}
}