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
    
    // create MainWindow
    MainWindow w;
    w.update_wallpapers();
    
    // start ScreenMonitor
    ScreenMonitor sm(&w);
    
    // show MainWindow for Debugging
    w.show();
    
    // clear potentially left over ./core_cache file
    // Cache_ReaderW cleaner(GlobTools::getCurrExePathW()+L"core_cache");
    CleanCache(L"core_cache");
    
    
    
    // execute QApplication
    return a.exec();
}
