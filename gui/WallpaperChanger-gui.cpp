// Qt Libs
#include <QApplication>
#include <QSystemTrayIcon>
#include <QtPlugin>

// C++ STL
#include <windows.h>

// others
#include "resources.h"
#include "mainwindow.h"


int APIENTRY WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR     lpCmdLine,
    int       nShowCmd
)
{
    Q_INIT_RESOURCE(resources);
    
    int argc = 0;
    char** argv = nullptr;
    
    QApplication a(argc, argv);
    
    MainWindow w;
    w.show();
    
    return a.exec();
}
