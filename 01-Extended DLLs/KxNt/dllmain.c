#include "buildcfg.h"
#include <KexComm.h>

BOOL WINAPI DllMain(
	IN	PVOID		DllBase,
	IN	ULONG		Reason,
	IN	PCONTEXT	Context)
{
	if (Reason == DLL_PROCESS_ATTACH) {
		LdrDisableThreadCalloutsForDll(DllBase);
	}

	return TRUE;
}

NTSTATUS NTAPI NtCancelWaitCompletionPacket(
	IN	HANDLE	WaitCompletionPacketHandle,
	IN	BOOLEAN	RemoveSignaledPacket)
{
	return STATUS_NOT_IMPLEMENTED;
}

NTSTATUS NTAPI NtAssociateWaitCompletionPacket(
	IN	HANDLE		WaitCompletionPacketHandle,
	IN	HANDLE		IoCompletionHandle,
	IN	HANDLE		TargetObjectHandle,
	IN	PVOID		KeyContext OPTIONAL,
	IN	PVOID		ApcContext OPTIONAL,
	IN	NTSTATUS	IoStatus,
	IN	ULONG_PTR	IoStatusInformation,
	OUT	PBOOLEAN AlreadySignaled OPTIONAL)
{
	return STATUS_NOT_IMPLEMENTED;
}

NTSTATUS NTAPI NtCreateWaitCompletionPacket(
	OUT	PHANDLE		WaitCompletionPacketHandle,
	IN	ACCESS_MASK	DesiredAccess,
	IN	POBJECT_ATTRIBUTES ObjectAttributes OPTIONAL)
{
	return STATUS_NOT_IMPLEMENTED;
}

NTSTATUS NTAPI NtQueryWnfStateData(
	INT_PTR		pStateName,
	INT_PTR		pTypeId,
	INT_PTR		pExplicitScope,
	OUT UINT	nChangeStamp,
	OUT INT_PTR	pBuffer,
	PULONG		nBufferSize)
{
	return STATUS_NOT_IMPLEMENTED;
}

BOOL NTAPI IsApiSetImplemented(
	PCSTR	Contract)
{
	return FALSE;
}

NTSTATUS NTAPI RtlGetAppContainerParent(
	IN	PSID	AppContainerSid,
	OUT	PSID*	AppContainerSidParent)
{
	return STATUS_NOT_IMPLEMENTED;
}

typedef enum _APPCONTAINER_SID_TYPE
{
	NotAppContainerSidType,
	ChildAppContainerSidType,
	ParentAppContainerSidType,
	InvalidAppContainerSidType,
	MaxAppContainerSidType
} APPCONTAINER_SID_TYPE, *PAPPCONTAINER_SID_TYPE;

NTSTATUS NTAPI RtlGetAppContainerSidType(
	IN	PSID					AppContainerSid,
	OUT	PAPPCONTAINER_SID_TYPE	AppContainerSidType)
{
	return STATUS_NOT_IMPLEMENTED;
}

typedef struct _PS_PKG_CLAIM
{
	ULONG Flags;  // PSM_ACTIVATION_TOKEN_*
	ULONG Origin; // PackageOrigin
} PS_PKG_CLAIM, * PPS_PKG_CLAIM;

typedef GUID* PGUID;

NTSTATUS NTAPI RtlQueryPackageClaims(
	IN		HANDLE	TokenHandle,
	OUT		PWSTR	PackageFullName,
	IN OUT	PSIZE_T PackageSize,
	OUT		PWSTR AppId,
	IN OUT	PSIZE_T AppIdSize OPTIONAL,
	OUT		PGUID DynamicId OPTIONAL,
	OUT		PPS_PKG_CLAIM PkgClaim OPTIONAL,
	OUT		PULONG64 AttributesPresent OPTIONAL)
{
	return STATUS_NOT_IMPLEMENTED;
}