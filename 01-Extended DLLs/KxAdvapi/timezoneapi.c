#include "buildcfg.h"
#include <KexComm.h>

DWORD WINAPI GetDynamicTimeZoneInformationEffectiveYears(
    IN  const   PDYNAMIC_TIME_ZONE_INFORMATION TimeZoneInformation,
    OUT         LPDWORD                        FirstYear,
    OUT         LPDWORD                        LastYear)
{
    HKEY TimeZonesKey, TimeZoneKey, DynamicDstKey;
    DWORD Type, Count, Result = ERROR_SUCCESS;

    if ((FirstYear == NULL) || (LastYear == NULL)) {
        return ERROR_INVALID_PARAMETER;
    }

    if (RegCreateKeyExW(HKEY_LOCAL_MACHINE, L"Software\\Microsoft\\Windows NT\\CurrentVersion\\Time Zones",
        0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &TimeZonesKey, NULL)) {
        return ERROR_FILE_NOT_FOUND;
    }

    if (RegOpenKeyExW(TimeZonesKey, TimeZoneInformation->TimeZoneKeyName, 0, KEY_ALL_ACCESS, &TimeZoneKey)) {
        RegCloseKey(TimeZonesKey);
        return ERROR_FILE_NOT_FOUND;
    }

    if (RegOpenKeyExW(TimeZoneKey, L"Dynamic DST", 0, KEY_ALL_ACCESS, &DynamicDstKey)) {
        RegCloseKey(TimeZoneKey);
        RegCloseKey(TimeZonesKey);
        return ERROR_FILE_NOT_FOUND;
    }

    Count = sizeof(DWORD);
    if (RegQueryValueExW(DynamicDstKey, L"FirstEntry", NULL, &Type, (LPBYTE)FirstYear, &Count) || (Type != REG_DWORD) ||
        RegQueryValueExW(DynamicDstKey, L"LastEntry", NULL, &Type, (LPBYTE)LastYear, &Count) || (Type != REG_DWORD)) {
        Result = ERROR_FILE_NOT_FOUND;
    }

    RegCloseKey(DynamicDstKey);
    RegCloseKey(TimeZoneKey);
    RegCloseKey(TimeZonesKey);
    return Result;
}

typedef struct _REG_TZI_FORMAT
{
    LONG Bias;
    LONG StandardBias;
    LONG DaylightBias;
    SYSTEMTIME StandardDate;
    SYSTEMTIME DaylightDate;
} REG_TZI_FORMAT;

DWORD WINAPI EnumDynamicTimeZoneInformation(
    IN  DWORD                           Index,
    OUT PDYNAMIC_TIME_ZONE_INFORMATION  TimeZoneInformation)
{
    HKEY TimeZonesKey, TimeZoneKey;
    LSTATUS Result;
    DWORD Type, ZoneKeySize = 256, NameSize = 64, TimeZoneFormatSize = sizeof(REG_TZI_FORMAT);
    REG_TZI_FORMAT TimeZoneFormat;

    if (TimeZoneInformation == NULL || Index < 0) {
        return ERROR_INVALID_PARAMETER;
    }

    if (RegCreateKeyExW(HKEY_LOCAL_MACHINE, L"Software\\Microsoft\\Windows NT\\CurrentVersion\\Time Zones",
        0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &TimeZonesKey, NULL)) {
        return ERROR_NO_MORE_ITEMS;
    }

    Result = RegEnumKeyExW(TimeZonesKey, Index, TimeZoneInformation->TimeZoneKeyName, &ZoneKeySize, NULL, NULL, NULL, NULL);
    if (Result != ERROR_SUCCESS) {
        return Result;
    }

    Result = RegOpenKeyExW(TimeZonesKey, TimeZoneInformation->TimeZoneKeyName, 0, KEY_ALL_ACCESS, &TimeZoneKey);
    if (Result != ERROR_SUCCESS) {
        RegCloseKey(TimeZonesKey);
        return Result;
    }

    RegQueryValueExW(TimeZoneKey, L"Std", NULL, &Type, (LPBYTE)TimeZoneInformation->StandardName, &NameSize);
    RegQueryValueExW(TimeZoneKey, L"Dlt", NULL, &Type, (LPBYTE)TimeZoneInformation->DaylightName, &NameSize);
    RegQueryValueExW(TimeZoneKey, L"TZI", NULL, &Type, (LPBYTE)&TimeZoneFormat, &TimeZoneFormatSize);

    TimeZoneInformation->Bias = TimeZoneFormat.Bias;
    TimeZoneInformation->StandardBias = TimeZoneFormat.StandardBias;
    TimeZoneInformation->DaylightBias = TimeZoneFormat.DaylightBias;
    TimeZoneInformation->StandardDate = TimeZoneFormat.StandardDate;
    TimeZoneInformation->DaylightDate = TimeZoneFormat.DaylightDate;
    TimeZoneInformation->DynamicDaylightTimeDisabled = FALSE;

    RegCloseKey(TimeZonesKey);
    return Result;
}
