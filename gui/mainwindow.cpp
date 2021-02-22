// Qt Libs
#include <QDebug>
#include <QMessageBox>
#include <QCoreApplication>

// C++ STL
#include <fstream>

// others
#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , trayIcon(new QSystemTrayIcon(this)),
    timer(), m_Config()
{
    // initialize config
    std::wstring config_path = GlobTools::getCurrExePathW();
    config_path += L"config.ini";
    try {
        m_Config.read(config_path);
    } catch(std::exception& err) { // cannot find config.ini under current dir
        // write a new config.ini
        write_default_config();
    }
    
    // check whether to update wallpapers
    bool is_land_port_wallpaper_empty = 
        m_Config.get(L"wallpaper", L"landscape_wallpaper").empty() ||
        m_Config.get(L"wallpaper", L"portrait_wallpaper").empty();
    if (!is_land_port_wallpaper_empty) { // no empty wallpaper, have both landscape & portrait
        update_wallpapers();
    }
    
    // Tray icon menu
    auto menu = this->createMenu();
    this->trayIcon->setContextMenu(menu);
    
    // App icon
    auto appIcon = QIcon(":/res/icon.png");
    this->trayIcon->setIcon(appIcon);
    this->setWindowIcon(appIcon);
    
    // Displaying the tray icon
    this->trayIcon->show();
    
    // Interaction
    connect(trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::iconActivated);
    
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
    
    // wallpaper
    m_Config.addOption(L"wallpaper", L"default_wallpaper");
    m_Config.addOption(L"wallpaper", L"landscape_wallpaper");
    m_Config.addOption(L"wallpaper", L"portrait_wallpaper");
    // m_Config.add(L"wallpaper", L"default_wallpaper", L"");
    // m_Config.add(L"wallpaper", L"landscape_wallpaper", L"");
    // m_Config.add(L"wallpaper", L"portrait_wallpaper", L"");
    
    // write config file
    m_Config.write(L"./config.ini");
}
void MainWindow::update_wallpapers()
{
    QObject *parent = new QObject();
    
    QString program = QString::fromWCharArray(m_Config.get(L"program", L"core_program").c_str());
    QStringList arguments;
    arguments 
            // update landscape wallpaper
            << QString::fromWCharArray(L"--paste")
            << QString::fromWCharArray(m_Config.get(L"wallpaper", L"landscape_wallpaper").c_str())
            << QString::fromWCharArray(L"LANDSCAPE")
            // update portrait wallpaper
            << QString::fromWCharArray(L"--paste")
            << QString::fromWCharArray(m_Config.get(L"wallpaper", L"portrait_wallpaper").c_str())
            << QString::fromWCharArray(L"PORTRAIT")
    ;
    
    QProcess *myProcess = new QProcess(parent);
    myProcess->start(program, arguments);}

// void test()
// {
//     std::ofstream output("test.txt", std::ios::app);
//     output << "output test string\n";
//     output.close();
// }

void test()
{
    MessageBoxW(0, L"Hello", L"Information", 0);
}

QMenu* MainWindow::createMenu()
{
    auto main_menu = new QMenu(this);
    
    // Sub Menu >
    auto sub_menu = menu_add_menu(main_menu, L"Sub Menu");
        // Sub Action
        menu_add_action(sub_menu, L"Sub Action", test);
    
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

template<class FUNC>
void MainWindow::menu_add_action(
    QMenu* menu,
    const std::wstring& action_name,
    FUNC action_func)
{
    QAction* act = new QAction(QString::fromWCharArray(action_name.c_str()));
    connect(act, &QAction::triggered, qApp, action_func);
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
