#include "buildcfg.h"
#include "kxuiacorep.h"

typedef enum NotificationKind {
    NotificationKind_ItemAdded = 0,
    NotificationKind_ItemRemoved = 1,
    NotificationKind_ActionCompleted = 2,
    NotificationKind_ActionAborted = 3,
    NotificationKind_Other = 4
} NotificationKind;

typedef enum NotificationProcessing {
    NotificationProcessing_ImportantAll = 0,
    NotificationProcessing_ImportantMostRecent = 1,
    NotificationProcessing_All = 2,
    NotificationProcessing_MostRecent = 3,
    NotificationProcessing_CurrentThenMostRecent = 4
} NotificationProcessing;

typedef interface IRawElementProviderSimple IRawElementProviderSimple;

HRESULT UiaRaiseNotificationEvent(
    IN  IRawElementProviderSimple*  Provider,
        NotificationKind            NotificationKind,
        NotificationProcessing      NotificationProcessing,
    IN  BSTR                        DisplayString OPTIONAL,
    IN  BSTR                        ActivityId)
{
    KexLogWarningEvent(L"Unimplemented API UiaRaiseNotificationEvent called");
    KexDebugCheckpoint();
    return S_OK;
}
