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
    
    
    Cache_ReaderW cleaner;
    
    
    
    // Cache_ReaderW reader;
    // auto ptr = reader.getData();
    
    // if (reader.isCacheExist()) {
    //     std::wstring out;
    //     for (auto it : *ptr)
    //         out += it + L'\n';
        
    //     MessageBoxW(0, out.c_str(), L"Info", 0);
    // }
    
    
    // execute QApplication
    return a.exec();
}
