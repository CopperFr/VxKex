#include "buildcfg.h"
#include <KexComm.h>

LSTATUS WINAPI Ext_RegGetValueW(
	IN      HKEY    hkey,
	IN      LPCWSTR lpSubKey OPTIONAL,
	IN      LPCWSTR lpValue OPTIONAL,
	IN      DWORD   dwFlags OPTIONAL,
	OUT		LPDWORD pdwType OPTIONAL,
	OUT     PVOID   pvData OPTIONAL,
	IN OUT	LPDWORD pcbData)
{
	LSTATUS Status;
	DWORD dwOldFlags = 0;
	DWORD dwType;

	if (((dwFlags & RRF_RT_ANY) != RRF_RT_ANY) &&
		(dwFlags & RRF_RT_REG_EXPAND_SZ) &&
		!(dwFlags & RRF_NOEXPAND))
	{		
		dwOldFlags = dwFlags;
		dwFlags |= RRF_RT_ANY;
		if (pdwType == NULL) {
			pdwType = &dwType;
		}
	}

	Status = RegGetValueW(hkey, lpSubKey, lpValue, dwFlags, pdwType, pvData, pcbData);
	if ((Status == ERROR_SUCCESS) && dwOldFlags) {
		DWORD dwAskedType;
		Status = RegGetValueW(hkey, lpSubKey, lpValue, dwOldFlags | RRF_NOEXPAND, &dwAskedType, NULL, NULL);
		if ((Status == ERROR_SUCCESS) && dwAskedType != *pdwType) {
			Status = ERROR_FILE_NOT_FOUND;
		}
	}
	return Status;
}

LSTATUS WINAPI Ext_RegGetValueA(
	IN      HKEY    hkey,
	IN      LPCSTR lpSubKey OPTIONAL,
	IN      LPCSTR lpValue OPTIONAL,
	IN      DWORD   dwFlags OPTIONAL,
	OUT		LPDWORD pdwType OPTIONAL,
	OUT     PVOID   pvData OPTIONAL,
	IN OUT	LPDWORD pcbData)
{
	LSTATUS Status;
	DWORD dwOldFlags = 0;
	DWORD dwType;

	if (((dwFlags & RRF_RT_ANY) != RRF_RT_ANY) &&
		(dwFlags & RRF_RT_REG_EXPAND_SZ) &&
		!(dwFlags & RRF_NOEXPAND))
	{
		dwOldFlags = dwFlags;
		dwFlags |= RRF_RT_ANY;
		if (pdwType == NULL) {
			pdwType = &dwType;
		}
	}

	Status = RegGetValueA(hkey, lpSubKey, lpValue, dwFlags, pdwType, pvData, pcbData);
	if ((Status == ERROR_SUCCESS) && dwOldFlags) {
		DWORD dwAskedType;
		Status = RegGetValueA(hkey, lpSubKey, lpValue, dwOldFlags | RRF_NOEXPAND, &dwAskedType, NULL, NULL);
		if ((Status == ERROR_SUCCESS) && dwAskedType != *pdwType) {
			Status = ERROR_FILE_NOT_FOUND;
		}
	}
	return Status;
}