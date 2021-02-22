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
    m_Config.add(L"system", L"windows_theme_dir", get_windows_sys_theme_dir());
    auto res = get_physical_screen_res();
    m_Config.add(L"system", L"horz_res", std::to_wstring(res.first));
    m_Config.add(L"system", L"vert_res", std::to_wstring(res.second));
    m_Config.write(L"./config.ini");
}

void test()
{
    std::ofstream output("test.txt", std::ios::app);
    output << "output test string\n";
    output.close();
}

QMenu* MainWindow::createMenu()
{
    // App can exit via Quit menu
    auto quitAction = new QAction("&Quit", this);
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
    
    // add menu
    auto menu = new QMenu(this);
    menu->addAction(quitAction);
    
    menu->addAction("test", test);
    
    return menu;
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

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason_)
{
    switch (reason_) {
    case QSystemTrayIcon::Trigger:
        this->trayIcon->showMessage("Hello", "You clicked me!");
        break;
    default:
        ;
    }
}
