// Qt Libs
#include <QApplication>
#include <QSystemTrayIcon>
#include <QtPlugin>
#include <QString>

// #include <QDir>
// #include <QDesktopWidget>
// #include <QRect>
// #include <QList>

#include <QSettings>
// #include <QVariant>

#include <QProcess>

#include <fstream>
#include "GlobTools.h"

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
    
    // QObject *parent = new QObject();
    
    // QString program = QString::fromWCharArray(L"./WallpaperChanger-core.exe");
    // QStringList arguments;
    // arguments << QString::fromWCharArray(L"--add")
    //         << QString::fromWCharArray(L"C:\\Users\\gyh18\\Desktop\\WallpaperChanger_rebuild\\wallpapers\\75225040_p0.jpg")
            
    //         << QString::fromWCharArray(L"--paste")
    //         << QString::fromWCharArray(L"000001")
    //         << QString::fromWCharArray(L"PORTRAIT")
    // ;
    
    // QProcess *myProcess = new QProcess(parent);
    // myProcess->start(program, arguments);
    
    
    // init wallpaper path
    std::wstring std_path(L"C:\\Users\\gyh18\\Desktop\\WallpaperChanger_rebuild\\wallpapers\\75225040_p0.jpg");
    QString *path = new QString(QString::fromWCharArray(std_path.c_str()));
    // set registry wallpaper path value
	QSettings *settings = new QSettings("HKEY_CURRENT_USER\\Control Panel\\Desktop",
                            QSettings::NativeFormat);
	settings->setValue("Wallpaper", QVariant(*path));//"C:\\Documents and Settings\\All Users\\Documents\\My Pictures\\Sample Pictures\\Sunset.bmp");
    // set wallpaper
	SystemParametersInfoW(SPI_SETDESKWALLPAPER, 1, std_path.data(), SPIF_SENDWININICHANGE);
    
    
    
    // execute QApplication
    return a.exec();
}
