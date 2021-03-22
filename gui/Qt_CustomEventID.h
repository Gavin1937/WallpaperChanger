
// int IDs for Qt Custom Events

// custom event for ScreenMonitor
#define SCREEN_MODE_CHANGE_EVENT_TYPE             QEvent::Type(QEvent::User+1)

// custom event for CacheBrowserDlg
#define RELOAD_WALLPAPERS_EVENT_TYPE              QEvent::Type(QEvent::User+2)
#define ADD_FILE_FROM_COMPUTER_EVENT_TYPE         QEvent::Type(QEvent::User+3)
#define ADD_FILE_FROM_CACHE_EVENT_TYPE            QEvent::Type(QEvent::User+4)
#define REMOVE_CACHE_EVENT_TYPE                   QEvent::Type(QEvent::User+5)
#define EDIT_CACHE_EVENT_TYPE                     QEvent::Type(QEvent::User+6)
#define CACHE_INFO_EVENT_TYPE                     QEvent::Type(QEvent::User+7)