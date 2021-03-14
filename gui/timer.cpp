
// others
#include "timer.h"


// ====================== public function ======================

ScreenMode getScreenMode()
{
    // get primary screen resolution after zoom in/out
    int CX = GetSystemMetrics(SM_CXSCREEN);
    int CY = GetSystemMetrics(SM_CYSCREEN);
    // get all screens resolution after zoom in/out
    int virCX = GetSystemMetrics(SM_CXVIRTUALSCREEN);
    int virCY = GetSystemMetrics(SM_CYVIRTUALSCREEN);
    
    // determine screen mode
    if (CX == virCX && CY == virCY)
        return ScreenMode::Single;
    else if (CX != virCX || CY != virCY)
        return ScreenMode::Multi;
    else
        return ScreenMode::Unknow;
}

// ====================== public function End ======================





// ====================== WallpaperUpdater ======================

WallpaperUpdater::WallpaperUpdater(QWidget *parent)
    : QWidget(parent)
{
    // set up QTimer obj
    m_Timer = new QTimer(this);
    connect(m_Timer, &QTimer::timeout, this, &WallpaperUpdater::update_wallpapers);
    // get sec from config.ini
    ConfigManager loc_config(GlobTools::getCurrExePathW()+L"config.ini", false);
    m_Timer->start(loc_config.getInt(L"program", L"wallpaper_update_time")*1000);
}

void WallpaperUpdater::update_wallpapers()
{
    ConfigManager loc_config(GlobTools::getCurrExePathW()+L"config.ini", false);
    // rm -rf /CachedFiles/ first, and then re-create that dir 
    QDir theme_dir(QString::fromWCharArray(loc_config.get(L"system", L"windows_theme_dir").c_str()));
    QDir cache_dir(theme_dir.absolutePath()+QString::fromWCharArray(L"CachedFiles"));
    if (cache_dir.exists())
        cache_dir.removeRecursively();
    theme_dir.mkdir(QString::fromWCharArray(L"CachedFiles"));
    
    // update landscape wallpaper
    QObject *parent1 = new QObject(this);
    QString program1 = QString::fromWCharArray(loc_config.get(L"program", L"core_program").c_str());
    QStringList arguments1;
    arguments1
            << QString::fromWCharArray(L"--paste")
            << QString::fromWCharArray(loc_config.get(L"wallpaper", L"landscape_wallpaper_id").c_str())
            << QString::fromWCharArray(L"LANDSCAPE")
    ;
    QProcess *myProcess1 = new QProcess(parent1);
    myProcess1->start(program1, arguments1);
    
    // update portrait wallpaper
    QObject* parent2 = new QObject(this);
    QString program2 = QString::fromWCharArray(loc_config.get(L"program", L"core_program").c_str());
    QStringList arguments2;
    arguments2
            << QString::fromWCharArray(L"--paste")
            << QString::fromWCharArray(loc_config.get(L"wallpaper", L"portrait_wallpaper_id").c_str())
            << QString::fromWCharArray(L"PORTRAIT")
    ;
    QProcess *myProcess2 = new QProcess(parent2);
    myProcess2->start(program2, arguments2);
}


// ====================== WallpaperUpdater End ======================





// ====================== ScreenModeChangeEvent ======================

ScreenModeChangeEvent::ScreenModeChangeEvent()
    : QEvent(QEvent::Type(QEvent::User+1))
{}


// ====================== ScreenModeChangeEvent End ======================





// ====================== ScreenMonitor ======================

ScreenMonitor::ScreenMonitor(QObject* event_receiver, QWidget *parent)
    : m_EventReceiver(event_receiver), QWidget(parent), m_ScreenMode(ScreenMode::Unknow)
{
    m_ScreenMode = getScreenMode();
    
    // set up QTimer obj
    m_Timer = new QTimer(this);
    connect(m_Timer, &QTimer::timeout, this, &ScreenMonitor::detect_screen_mode);
    // set timer to run every 0.5 sec
    m_Timer->start(500);
}

void ScreenMonitor::detect_screen_mode()
{
    // old screen mode
    auto last_mode = m_ScreenMode;
    // set screen mode
    m_ScreenMode = getScreenMode();
    
    // post ScreenMode Change Event
    if (last_mode != m_ScreenMode) // screen mode changed, send update event
        QApplication::instance()->postEvent(m_EventReceiver, new ScreenModeChangeEvent());
}


// ====================== ScreenMonitor End ======================
