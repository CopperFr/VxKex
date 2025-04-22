#include "buildcfg.h"
#include "kxbasep.h"

KXBASEAPI LONG WINAPI GetCurrentPackageFullName(
	IN OUT	PULONG	PackageFullNameLength,
	OUT		PWSTR	PackageFullName OPTIONAL)
{
	KexLogWarningEvent(L"GetCurrentPackageFullName called");

	if (!PackageFullNameLength) {
		return ERROR_INVALID_PARAMETER;
	}

	if (*PackageFullNameLength != 0 && !PackageFullName) {
		return ERROR_INVALID_PARAMETER;
	}

	return APPMODEL_ERROR_NO_PACKAGE;
}

KXBASEAPI LONG WINAPI GetCurrentPackageId(
	IN OUT	PULONG	BufferLength,
	OUT		PBYTE	Buffer OPTIONAL)
{
	KexLogWarningEvent(L"GetCurrentPackageId called");

	if (!BufferLength) {
		return ERROR_INVALID_PARAMETER;
	}

	if (*BufferLength != 0 && !Buffer) {
		return ERROR_INVALID_PARAMETER;
	}

	return APPMODEL_ERROR_NO_PACKAGE;
}

KXBASEAPI LONG WINAPI GetCurrentPackageInfo(
	IN		const UINT32	Flags,
	IN OUT	PUINT32			BufferLength,
	OUT		PBYTE			Buffer OPTIONAL,
	OUT		PUINT32			Count OPTIONAL)
{
	KexLogWarningEvent(L"GetCurrentPackageInfo called");

	if (!BufferLength) {
		return ERROR_INVALID_PARAMETER;
	}

	if (*BufferLength != 0 && !Buffer) {
		return ERROR_INVALID_PARAMETER;
	}

	return APPMODEL_ERROR_NO_PACKAGE;
}

KXBASEAPI LONG WINAPI GetCurrentPackageFamilyName(
	IN OUT	PUINT32	PackageFamilyNameLength,
	OUT		PWSTR	PackageFamilyName)
{
	KexLogWarningEvent(L"GetCurrentPackageFamilyName called");

	if (!PackageFamilyNameLength) {
		return ERROR_INVALID_PARAMETER;
	}

	if (*PackageFamilyNameLength != 0 && !PackageFamilyName) {
		return ERROR_INVALID_PARAMETER;
	}

	return APPMODEL_ERROR_NO_PACKAGE;
}

KXBASEAPI LONG WINAPI GetPackageFullName(
	IN		HANDLE	ProcessHandle,
	IN OUT	PULONG	NameLength,
	OUT		PWSTR	PackageFullName OPTIONAL)
{
	KexLogWarningEvent(L"GetPackageFullName called: %d", ProcessHandle);
	
	if (!ProcessHandle || !NameLength || *NameLength && !PackageFullName) {
		return ERROR_INVALID_PARAMETER;
	}

	*NameLength = 0;

	if (PackageFullName) {
		PackageFullName[0] = '\0';
	}

	return APPMODEL_ERROR_NO_PACKAGE;
}

KXBASEAPI LONG WINAPI GetPackageInfo(
	IN		PACKAGE_INFO_REFERENCE	PackageInfoReference,
	IN		const UINT32			Flags,
	IN OUT	PUINT32					BufferLength,
	OUT		PBYTE					Buffer,
	OUT		PUINT32					Count)
{
	KexLogWarningEvent(L"GetPackageInfo called");

	if (!BufferLength) {
		return ERROR_INVALID_PARAMETER;
	}

	if (*BufferLength != 0 && !Buffer) {
		return ERROR_INVALID_PARAMETER;
	}

	return APPMODEL_ERROR_NO_PACKAGE;
}

KXBASEAPI LONG WINAPI GetPackageFamilyName(
	IN		HANDLE	ProcessHandle,
	IN OUT	PULONG	NameLength,
	OUT		PWSTR	PackageFamilyName OPTIONAL)
{
	KexLogWarningEvent(L"GetPackageFamilyName called: %d", ProcessHandle);
	
	if (!ProcessHandle || !NameLength || *NameLength && !PackageFamilyName) {
		return ERROR_INVALID_PARAMETER;
	}

	*NameLength = 0;

	if (PackageFamilyName) {
		PackageFamilyName[0] = '\0';
	}

	return APPMODEL_ERROR_NO_PACKAGE;
}

KXBASEAPI LONG WINAPI GetPackagePathByFullName(
	IN		PCWSTR	PackageFullName,
	IN OUT	PUINT32	PathLength,
	OUT		PWSTR  Path OPTIONAL)
{
	KexLogWarningEvent(L"GetPackagePathByFullName called: %s", PackageFullName);
	
	if (!PackageFullName || !PathLength) {
		return ERROR_INVALID_PARAMETER;
	}
	
	*PathLength = 0;

	if (Path) {
		Path[0] = '\0';
	}
	
	return APPMODEL_ERROR_NO_PACKAGE;
}

KXBASEAPI LONG WINAPI GetStagedPackagePathByFullName(
	IN		PCWSTR	PackageFullName,
	IN OUT	PUINT32	PathLength,
	OUT		PWSTR  Path OPTIONAL)
{
	KexLogWarningEvent(L"GetStagedPackagePathByFullName called: %s", PackageFullName);

	if (!PackageFullName || !PathLength) {
		return ERROR_INVALID_PARAMETER;
	}

	*PathLength = 0;

	if (Path) {
		Path[0] = '\0';
	}

	return APPMODEL_ERROR_NO_PACKAGE;
}

// Unkown Signature
KXBASEAPI LONG WINAPI GetSystemMetadataPathForPackage(
	IN		PCWSTR	PackageFullName,
	IN OUT	PUINT32	PathLength,
	OUT		PWSTR  Path OPTIONAL)
{
	KexLogWarningEvent(L"GetSystemMetadataPathForPackage called: %s", PackageFullName);

	if (!PackageFullName || !PathLength) {
		return ERROR_INVALID_PARAMETER;
	}

	*PathLength = 0;

	if (Path) {
		Path[0] = '\0';
	}

	return APPMODEL_ERROR_NO_PACKAGE;
}

// Unkown Signature
KXBASEAPI LONG WINAPI GetEffectivePackageStatusForUserSid()
{
	KexLogWarningEvent(L"GetEffectivePackageStatusForUserSid called");
	return APPMODEL_ERROR_NO_PACKAGE;
}

KXBASEAPI LONG WINAPI GetPackagesByPackageFamily(
	IN		PCWSTR	PackageFamilyName,
	IN OUT	PULONG	Count,
	OUT		PPWSTR	PackageFullNames OPTIONAL,
	IN OUT	PULONG	BufferLength,
	OUT		PPWSTR	Buffer OPTIONAL)
{
	KexLogWarningEvent(L"GetPackagesByPackageFamily called: %s", PackageFamilyName);
	
	if (!Count || !BufferLength) {
		return ERROR_INVALID_PARAMETER;
	}

	*Count = 0;
	*BufferLength = 0;

	return ERROR_SUCCESS;
}

KXBASEAPI LONG WINAPI FindPackagesByPackageFamily(
	IN		PCWSTR	PackageFamilyName,
	IN		UINT32	PackageFilters,
	IN OUT  PUINT32 Count,
	OUT		PPWSTR	PackageFullNames OPTIONAL,
	IN OUT  PUINT32	BufferLength,
	OUT		PWCHAR	Buffer OPTIONAL,
	OUT		PUINT32 PackageProperties OPTIONAL)
{
	KexLogWarningEvent(L"FindPackagesByPackageFamily called: %s", PackageFamilyName);
	if (!Count || !BufferLength) {
		return ERROR_INVALID_PARAMETER;
	}

	*Count = 0;
	*BufferLength = 0;

	return ERROR_SUCCESS;
}

KXBASEAPI LONG WINAPI AppPolicyGetProcessTerminationMethod(
	IN	HANDLE	ProcessToken,
	OUT	PULONG	Policy)
{
	if (!ProcessToken || !Policy) {
		return RtlNtStatusToDosError(STATUS_INVALID_PARAMETER);
	}

	*Policy = 0;
	return ERROR_SUCCESS;
}

KXBASEAPI HRESULT WINAPI AppXGetPackageSid(
	IN	PCWSTR	PackageMoniker,
	OUT	PSID	*PackageSid)
{
	KexLogWarningEvent(L"AppXGetPackageSid called: %s", PackageMoniker);
	KexDebugCheckpoint();

	if (!PackageMoniker || !PackageSid) {
		return E_INVALIDARG;
	}

	return E_NOTIMPL;
}

KXBASEAPI VOID WINAPI AppXFreeMemory(
	IN	PVOID	Pointer)
{
	RtlFreeHeap(RtlProcessHeap(), 0, Pointer);
}

KXBASEAPI ULONG WINAPI PackageIdFromFullName(
	IN		PCWSTR			PackageFullName,
	IN		const UINT32	Flags,
	IN OUT	PUINT32			BufferLength,
	OUT		PBYTE			Buffer OPTIONAL)
{
	KexLogWarningEvent(L"PackageIdFromFullName called: %s", PackageFullName);

	if (!PackageFullName || !BufferLength) {
		return ERROR_INVALID_PARAMETER;
	}
	
	if (*BufferLength != 0 && !Buffer) {
		return ERROR_INVALID_PARAMETER;
	}

	return ERROR_SUCCESS;
}

KXBASEAPI ULONG WINAPI PackageFamilyNameFromFullName(
	IN		PCWSTR	PackageFullName,
	IN OUT	PULONG	PackageFamilyNameLength,
	OUT		PWSTR	PackageFamilyName)
{
	KexLogWarningEvent(L"PackageFamilyNameFromFullName called: %s", PackageFullName);

	if (!PackageFullName || !PackageFamilyNameLength) {
		return ERROR_INVALID_PARAMETER;
	}

	if (*PackageFamilyNameLength != 0 && !PackageFamilyName) {
		return ERROR_INVALID_PARAMETER;
	}

	if (*PackageFamilyNameLength != 0) {
		PackageFamilyName[0] = '\0';
	}

	*PackageFamilyNameLength = 0;
	return ERROR_SUCCESS;
}

KXBASEAPI LONG WINAPI PackageFamilyNameFromId(
	IN      const PPACKAGE_ID	PackageId,
	IN OUT  PUINT32				PackageFamilyNameLength,
	OUT		PWSTR				PackageFamilyName)
{
	KexLogWarningEvent(L"PackageFamilyNameFromId called: %s", PackageId->name);

	if (!PackageId || !PackageFamilyNameLength || *PackageFamilyNameLength && !PackageFamilyName) {
		return ERROR_INVALID_PARAMETER;
	}

	*PackageFamilyNameLength = 0;

	if (PackageFamilyName) {
		PackageFamilyName[0] = '\0';
	}

	return APPMODEL_ERROR_NO_PACKAGE;
}

KXBASEAPI LONG WINAPI OpenPackageInfoByFullName(
	IN	PCWSTR		PackageFullName,
	const UINT32	Reserved,
	OUT	PPACKAGE_INFO_REFERENCE PackageInfoReference)
{
	KexLogWarningEvent(L"OpenPackageInfoByFullName called: %s", PackageFullName);
	return ERROR_NOT_FOUND;
}

KXBASEAPI LONG WINAPI ClosePackageInfo(
	IN	PACKAGE_INFO_REFERENCE	packageInfoReference)
{
	return ERROR_SUCCESS;
}

KXBASEAPI LONG WINAPI GetPackageApplicationIds(
	IN		PACKAGE_INFO_REFERENCE	PackageInfoReference,
	IN OUT	PUINT32					BufferLength,
	OUT		PBYTE					Buffer OPTIONAL,
	OUT		PUINT32					Count OPTIONAL)
{
	return APPMODEL_ERROR_NO_APPLICATION;
}

KXBASEAPI LONG WINAPI GetApplicationUserModelId(
	IN		HANDLE	ProcessHandle,
	IN OUT	PULONG	ApplicationUserModelIdLength,
	OUT		PWSTR	ApplicationUserModelId)
{
	return APPMODEL_ERROR_NO_APPLICATION;
}

KXBASEAPI LONG WINAPI GetCurrentApplicationUserModelId(
	IN OUT	PULONG	Cch,
	OUT		PWSTR	Buffer)
{
	return APPMODEL_ERROR_NO_APPLICATION;
}

KXBASEAPI LONG WINAPI AppPolicyGetWindowingModel(
	IN	HANDLE						ProcessToken,
	OUT	PAPP_POLICY_WINDOWING_MODEL	WindowingModel)
{
	if (!ProcessToken || !WindowingModel) {
		return ERROR_INVALID_PARAMETER;
	}

	*WindowingModel = AppPolicyWindowingModel_ClassicDesktop;
	return ERROR_SUCCESS;
}

KXBASEAPI LONG WINAPI AppPolicyGetThreadInitializationType(
	IN	HANDLE									ProcessToken,
	OUT	PAPP_POLICY_THREAD_INITIALIZATION_TYPE	InitializationType)
{
	if (!ProcessToken || !InitializationType) {
		return ERROR_INVALID_PARAMETER;
	}

	*InitializationType = AppPolicyThreadInitializationType_None;
	return ERROR_SUCCESS;
}