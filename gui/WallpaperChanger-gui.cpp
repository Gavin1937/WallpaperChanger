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
#include "../utilities/GlobTools.h"


int APIENTRY WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR     lpCmdLine,
    int       nShowCmd)
{
    // clear potentially left over ./core_cache file
    CleanCache(GlobTools::getCurrExePathW()+L"core_cache");
    // clear potentially left over ./cropped_img.BMP
    CleanCache(GlobTools::getCurrExePathW()+L"cropped_img.BMP");
    
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
        w.MyShow();
    
    // start ScreenMonitor
    ScreenMonitor sm(&w);
    
    // execute QApplication
    int qapp_result = a.exec();
    
    // clear potentially left over ./core_cache file
    CleanCache(GlobTools::getCurrExePathW()+L"core_cache");
    // clear potentially left over ./cropped_img.BMP
    CleanCache(GlobTools::getCurrExePathW()+L"cropped_img.BMP");
    
    return qapp_result;
}
