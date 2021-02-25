#pragma once

// Qt Libs
#include <QWidget>
#include <QString>
#include <QProcess>
#include <QTimer>
#include <QDir>
#include <QEvent>
// #include <QCoreApplication>
#include <QApplication>

// Windows API
#include <Windows.h>

// other
#include "../utilities/ConfigManager.h"
#include "../utilities/GlobTools.h"


// update wallpaper autometically
class WallpaperUpdater : public QWidget
{
    Q_OBJECT
    
public:
    WallpaperUpdater(QWidget *parent = 0);
    
public slots:
    void update_wallpapers();
    
private:
    QTimer* m_Timer;
};


// Screen Mode, Multi-Screen or Single-Screen
enum class ScreenMode {
    Unknow = 0, Multi, Single
};

// custom event
class ScreenModeChangeEvent : public QEvent
{
public:
    ScreenModeChangeEvent();
};

// detect screen resize / screen mode(multi/single) change & postEvent
class ScreenMonitor : public QWidget
{
    Q_OBJECT
    
public:
    ScreenMonitor(QObject* event_receiver = 0, QWidget *parent = 0);
    
public slots:
    void detect_screen_mode();
    
private:
    QTimer* m_Timer;
    ScreenMode m_ScreenMode;
    QObject* m_EventReceiver;
};
