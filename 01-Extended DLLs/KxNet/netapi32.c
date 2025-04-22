#include "buildcfg.h"
#include "kxnetp.h"

KXNETAPI HRESULT NET_API_FUNCTION NetGetAadJoinInformation(
	IN 	LPCWSTR          pcszTenantId OPTIONAL,
	OUT	PDSREG_JOIN_INFO* ppJoinInfo)
{
	KexLogWarningEvent(L"Unimplemented API NetGetAadJoinInformation called");
	KexDebugCheckpoint();
	*ppJoinInfo = NULL;
	return S_OK;
}

VOID NET_API_FUNCTION NetFreeAadJoinInformation(
	IN PDSREG_JOIN_INFO pJoinInfo OPTIONAL)
{
	KexLogWarningEvent(L"Unimplemented API NetFreeAadJoinInformation called");
	KexDebugCheckpoint();
}