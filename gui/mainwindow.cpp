// Qt Libs
#include <QDebug>
#include <QMessageBox>
#include <QCoreApplication>

// C++ STL
#include <fstream>

// others
#include "mainwindow.h"


// ====================== MainWindow ======================

// public

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_TrayIcon(new QSystemTrayIcon(this)),
    m_Wallpaper_Updater(nullptr),
    m_TrayIconMenu(nullptr), m_Config()
{
    // initialize config
    std::wstring config_path = GlobTools::getCurrExePathW();
    config_path += L"config.ini";
    try {
        m_Config.read(config_path);
    } catch(std::exception& err) { // cannot find config.ini under current dir
        // write a new config.ini
        write_default_config();
        // prompt user to input basic configs
        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        // ! NEED TO REPLACE WITH PROPER DIALOG PROMPT
        MessageBoxW(0, L"Missing Components in config.ini", L"Warning", 0);
        return;
    }
    
    // check whether to update wallpapers
    bool is_land_port_wallpaper_empty = 
        m_Config.get(L"wallpaper", L"landscape_wallpaper").empty() ||
        m_Config.get(L"wallpaper", L"portrait_wallpaper").empty();
    if (!is_land_port_wallpaper_empty) { // no empty wallpaper, have both landscape & portrait
        // set default wappaper and update wallpaper
        set_default_wallpaper();
    } else { // have empty wallpaper, prompt user for input
        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        // ! NEED TO REPLACE WITH PROPER DIALOG PROMPT
        MessageBoxW(0, L"Missing Components in config.ini", L"Warning", 0);
        return;
        
        // // set default wappaper and update wallpaper
        // set_default_wallpaper();
    }
    
    // init timer
    m_Wallpaper_Updater = new WallpaperUpdater();
    
    // App icon
    auto appIcon = QIcon(":/res/icon.png");
    this->m_TrayIcon->setIcon(appIcon);
    this->setWindowIcon(appIcon);
    
    // Tray icon menu
    m_TrayIconMenu = this->createMenu();
    this->m_TrayIcon->setContextMenu(m_TrayIconMenu);
    
    // Displaying the tray icon
    this->m_TrayIcon->show();
    
    // Interaction
    connect(m_TrayIcon, &QSystemTrayIcon::activated, this, &MainWindow::iconActivated);
}

void MainWindow::write_default_config()
{
    // system
    m_Config.add(L"system", L"windows_theme_dir", get_windows_sys_theme_dir());
    auto res = get_physical_screen_res();
    m_Config.add(L"system", L"horz_res", std::to_wstring(res.first));
    m_Config.add(L"system", L"vert_res", std::to_wstring(res.second));
    
    // program
    m_Config.add(L"program", L"core_program", GlobTools::getCurrExePathW() + L"WallpaperChanger-core.exe");
    m_Config.add(L"program", L"gui_program", GlobTools::getCurrExePathW() + L"WallpaperChanger-gui.exe");
    m_Config.add(L"program", L"wallpaper_update_time", L"1800"); // val in seconds, default = 30min
    
    // wallpaper
    m_Config.add(L"wallpaper", L"default_wallpaper_path", L"");
    m_Config.add(L"wallpaper", L"landscape_wallpaper", L"");
    m_Config.add(L"wallpaper", L"portrait_wallpaper", L"");
    
    // write config file
    m_Config.write(L"./config.ini");
}

void MainWindow::update_wallpapers()
{
    // rm -rf /CachedFiles/ first, and then re-create that dir 
    QDir theme_dir(QString::fromWCharArray(m_Config.get(L"system", L"windows_theme_dir").c_str()));
    QDir cache_dir(theme_dir.absolutePath()+QString::fromWCharArray(L"CachedFiles"));
    if (cache_dir.exists())
        cache_dir.removeRecursively();
    theme_dir.mkdir(QString::fromWCharArray(L"CachedFiles"));
    
    // update landscape wallpaper
    QObject *parent1 = new QObject();
    QString program1 = QString::fromWCharArray(m_Config.get(L"program", L"core_program").c_str());
    QStringList arguments1;
    arguments1
            << QString::fromWCharArray(L"--paste")
            << QString::fromWCharArray(m_Config.get(L"wallpaper", L"landscape_wallpaper").c_str())
            << QString::fromWCharArray(L"LANDSCAPE")
    ;
    QProcess *myProcess1 = new QProcess(parent1);
    myProcess1->start(program1, arguments1);
    
    // update portrait wallpaper
    QObject* parent2 = new QObject();
    QString program2 = QString::fromWCharArray(m_Config.get(L"program", L"core_program").c_str());
    QStringList arguments2;
    arguments2
            << QString::fromWCharArray(L"--paste")
            << QString::fromWCharArray(m_Config.get(L"wallpaper", L"portrait_wallpaper").c_str())
            << QString::fromWCharArray(L"PORTRAIT")
    ;
    QProcess *myProcess2 = new QProcess(parent2);
    myProcess2->start(program2, arguments2);
}


// public slot:
void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason_)
{
    // switch (reason_) {
    // case QSystemTrayIcon::Trigger:
    //     this->trayIcon->showMessage("Hello", "You clicked me!");
    //     break;
    // default:
    //     ;
    // }
}


// private:

QMenu* MainWindow::createMenu()
{
    auto main_menu = new QMenu(this);
    
    // Update Wallpapers
    menu_add_action(main_menu, L"Update Wallpapers", &MainWindow::update_wallpapers);
    
    // // Sub Menu >
    // auto sub_menu = menu_add_menu(main_menu, L"Sub Menu");
    //     // Sub Action
    //     menu_add_action(sub_menu, L"Sub Action", test);
    
    // Quit
    menu_add_action(main_menu, L"Quit", &QCoreApplication::quit);
    
    return main_menu;
}

// helper functions

std::wstring MainWindow::get_windows_sys_theme_dir()
{
    std::wstring output = QDir::homePath().toStdWString();
    size_t temp = output.find(L'/');
    while (temp != std::wstring::npos) {
        output.replace(output.begin()+temp, output.begin()+temp+1, L"\\");
        temp = output.find(L'/');
    }
    output += L"\\AppData\\Roaming\\Microsoft\\Windows\\Themes\\";
    return output;
}
std::pair<int, int> MainWindow::get_physical_screen_res()
{
    // Get the monitor currently displayed in the window
    // Use the handle of the desktop.
    HWND hWnd = GetDesktopWindow();
    HMONITOR hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
    
    // Get the monitor logic width and height
    MONITORINFOEX miex;
    miex.cbSize = sizeof(miex);
    GetMonitorInfo(hMonitor, &miex);
    
    // Get the monitor physical width and height
    DEVMODE dm;
    dm.dmSize = sizeof(dm);
    dm.dmDriverExtra = 0;
    EnumDisplaySettings(miex.szDevice, ENUM_CURRENT_SETTINGS, &dm);
    int cxPhysical = dm.dmPelsWidth;
    int cyPhysical = dm.dmPelsHeight;
    
    return std::pair<int, int>(cxPhysical, cyPhysical);
}
void MainWindow::set_default_wallpaper()
{
    // init wallpaper path
    std::wstring default_wallpaper = m_Config.get(L"wallpaper", L"default_wallpaper_path");
    QString *path = new QString(QString::fromWCharArray(default_wallpaper.c_str()));
    // set registry wallpaper path value
	QSettings *settings = new QSettings("HKEY_CURRENT_USER\\Control Panel\\Desktop",
                            QSettings::NativeFormat);
	settings->setValue("Wallpaper", QVariant(*path));
    // set wallpaper
	if (SystemParametersInfoW(SPI_SETDESKWALLPAPER, 1, default_wallpaper.data(),
        SPIF_SENDWININICHANGE) != 0)
    { //success
        // SystemParametersInfoW() will delete /CachedFiles/ under Windows Theme Dir
        // So update /CachedFiles/ after success
        update_wallpapers();
    }
}

template<class FUNC>
void MainWindow::menu_add_action(
    QMenu* menu,
    const std::wstring& action_name,
    FUNC action_func)
{
    QAction* act = new QAction(QString::fromWCharArray(action_name.c_str()));
    connect(act, &QAction::triggered, this, action_func);
    menu->addAction(act);
}
QMenu* MainWindow::menu_add_menu(
    QMenu* parent_menu,
    const std::wstring& child_menu_name,
    QMenu* child_menu)
{
    if (child_menu == NULL || child_menu == nullptr)
        child_menu = new QMenu(this);
    child_menu->setTitle(QString::fromWCharArray(child_menu_name.c_str()));
    parent_menu->addMenu(child_menu);
    return child_menu;
}


// ====================== MainWindow End ======================