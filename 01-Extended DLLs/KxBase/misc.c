#include "buildcfg.h"
#include "kxbasep.h"

KXBASEAPI BOOL WINAPI GetOsSafeBootMode(
	OUT	PBOOL	IsSafeBootMode)
{
	*IsSafeBootMode = FALSE;
	return TRUE;
}

KXBASEAPI BOOL WINAPI GetFirmwareType(
	OUT	PFIRMWARE_TYPE	FirmwareType)
{
	*FirmwareType = FirmwareTypeUnknown;
	return TRUE;
}

// Unknown signature
KXBASEAPI BOOL WINAPI OpenGlobalizationUserSettingsKey()
{
	return FALSE;
}

// Unknown signature
KXBASEAPI BOOL WINAPI GetPersistedRegistryLocationW()
{
	return FALSE;
}