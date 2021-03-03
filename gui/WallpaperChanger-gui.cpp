// Qt Libs
#include <QApplication>
#include <QSystemTrayIcon>
#include <QtPlugin>
#include <QString>

// C++ STL
#include <string>

// Windows API
#include <windows.h>

// others
#include "resources.h"
#include "mainwindow.h"
#include "timer.h"
#include "../utilities/CacheRW.h"


int APIENTRY WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR     lpCmdLine,
    int       nShowCmd)
{
    // init .qrc resources file
    Q_INIT_RESOURCE(resources);
    
    // init QApplication
    int argc = 0;
    char** argv = nullptr;
    QApplication a(argc, argv);
    
    // set Application Style
    a.setStyle("fusion");
    
    // create MainWindow
    MainWindow w;
    
    // show MainWindow dlg when missing wallpaper
    if (!w.is_all_wallpaper_set())
        w.show();
    
    // start ScreenMonitor
    ScreenMonitor sm(&w);
    
    // clear potentially left over ./core_cache file
    CleanCache(L"core_cache");
    
    
    // execute QApplication
    return a.exec();
}
