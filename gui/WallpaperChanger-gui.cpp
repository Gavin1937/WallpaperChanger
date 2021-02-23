// Qt Libs
#include <QApplication>
#include <QSystemTrayIcon>
#include <QtPlugin>
#include <QString>

// C++ STL
#include <windows.h>

// others
#include "resources.h"
#include "mainwindow.h"


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
    
    // show MainWindow for Debugging
    w.show();
    
    
    // execute QApplication
    return a.exec();
}
