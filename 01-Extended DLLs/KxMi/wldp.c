#include "buildcfg.h"
#include "kxmip.h"

KXMIAPI HRESULT WINAPI WldpQueryWindowsLockdownMode(
	OUT	PWLDP_WINDOWS_LOCKDOWN_MODE	LockdownMode)
{
	ASSERT (LockdownMode != NULL);

	*LockdownMode = WLDP_WINDOWS_LOCKDOWN_MODE_UNLOCKED;
	return S_OK;
}

KXMIAPI HRESULT WldpGetLockdownPolicy(
	PWLDP_HOST_INFORMATION HostInformation,
	PDWORD                 LockdownState,
	DWORD                  LockdownFlags)
{
	*LockdownState = 0;
	return S_OK;
}