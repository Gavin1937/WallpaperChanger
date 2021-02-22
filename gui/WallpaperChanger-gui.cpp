// Qt Libs
#include <QApplication>
#include <QSystemTrayIcon>
#include <QtPlugin>
#include <QString>

// #include <QDir>
// #include <QDesktopWidget>
// #include <QRect>
// #include <QList>

#include <QProcess>

#include <fstream>

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
    w.show();
    
    // testing
    QObject *parent = new QObject();
    
    QString program = "./WallpaperChanger-core.exe";
    QStringList arguments;
    arguments << "--help";
    
    QProcess *myProcess = new QProcess(parent);
    myProcess->start(program, arguments);
    
    
    // execute QApplication
    return a.exec();
}
