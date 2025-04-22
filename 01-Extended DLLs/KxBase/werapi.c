#include "buildcfg.h"
#include "kxbasep.h"

KXBASEAPI HRESULT WINAPI WerRegisterCustomMetadata(
	PCWSTR key,
	PCWSTR value)
{
	// TODO Check Length
	if (key == NULL || value == NULL) {
		return E_INVALIDARG;
	}
	return S_OK;
}