#include "buildcfg.h"
#include "kexdllp.h"

typedef struct _PROCESS_TELEMETRY_ID_INFORMATION {
	ULONG HeaderSize;
	ULONG ProcessId;
	ULONG64 ProcessStartKey;
	ULONG64 CreateTime;
	ULONG64 CreateInterruptTime;
	ULONG64 CreateUnbiasedInterruptTime;
	ULONG64 ProcessSequenceNumber;
	ULONG64 SessionCreateTime;
	ULONG SessionId;
	ULONG BootId;
	ULONG ImageChecksum;
	ULONG ImageTimeDateStamp;
	ULONG UserSidOffset;
	ULONG ImagePathOffset;
	ULONG PackageNameOffset;
	ULONG RelativeAppNameOffset;
	ULONG CommandLineOffset;
} TYPEDEF_TYPE_NAME(PROCESS_TELEMETRY_ID_INFORMATION);


NTSTATUS NTAPI Ext_NtQueryInformationProcess(
	IN	HANDLE				ProcessHandle,
	IN	PROCESSINFOCLASS	ProcessInformationClass,
	OUT	PVOID				ProcessInformation,
	IN	ULONG				ProcessInformationLength,
	OUT	PULONG				ReturnLength OPTIONAL)
{
	if (ProcessInformationClass == 64) {

		KexLogWarningEvent(L"ProcessInformationClass == ProcessTelemetryIdInformation (64)");
		if (ProcessInformation) {
			((PPROCESS_TELEMETRY_ID_INFORMATION)ProcessInformation)->HeaderSize = sizeof(PROCESS_TELEMETRY_ID_INFORMATION);
			//((PPROCESS_TELEMETRY_ID_INFORMATION)ProcessInformation)->ProcessId = GetProcessId(ProcessHandle);
		}
		
		if (ReturnLength) {
			*ReturnLength = sizeof(PROCESS_TELEMETRY_ID_INFORMATION);
		}
		return ERROR_SUCCESS;
	}
	// TODO
	if (ProcessInformationClass == 58) {
		KexLogWarningEvent(L"ProcessInformationClass == 58");
		KexDebugCheckpoint();
	}

	if (ProcessInformationClass >= 51) {
		KexLogWarningEvent(L"ProcessInformationClass %d >= 51", ProcessInformationClass);
		KexDebugCheckpoint();
	}

	return NtQueryInformationProcess(
		ProcessHandle,
		ProcessInformationClass,
		ProcessInformation,
		ProcessInformationLength,
		ReturnLength);
}