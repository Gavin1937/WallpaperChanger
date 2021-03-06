
// TrayIcon part implementation of class MainWindow

#include "mainwindow.h"


// ====================== mainwindow_TrayIcon ======================

// TrayIcon slots
void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason_)
{
    switch (reason_) {
    case QSystemTrayIcon::DoubleClick: // show MainWindow Dialog
        mainwindowTab->setCurrentIndex(0);
        this->MyShow();
        break;
    default:
        break;
    }
}

// private:
void MainWindow::init_SysTrayIcon(QIcon* icon)
{
    // set icon for SysTrayIcon
    p_TrayIcon->setIcon(*icon);
    // Tray icon menu
    p_TrayIconMenu = createMenu();
    p_TrayIcon->setContextMenu(p_TrayIconMenu);
    QString tool_tip_msg =
        "WallpaperChanger\nv" + QString::number(PROGRAM_VERSION)
    ;
    p_TrayIcon->setToolTip(tool_tip_msg);
    // Displaying the tray icon
    p_TrayIcon->show();
    
    // connect SystemTrayIcon with activation handler
    connect(p_TrayIcon, &QSystemTrayIcon::activated, this, &MainWindow::iconActivated);
}
QMenu* MainWindow::createMenu()
{
    auto main_menu = new QMenu(this);
    
    // Update Wallpapers
    menu_add_action(main_menu, L"Update Wallpapers", &MainWindow::update_wallpapers);
    
    // Set Wallpapers
    menu_add_action(main_menu, L"Wallpaper Settings", &MainWindow::open_tab0);
    
    // Settings
    menu_add_action(main_menu, L"Program Settings", &MainWindow::open_tab1);
    
    // About
    menu_add_action(main_menu, L"About Program", &MainWindow::AboutProgramDlg);
    
    // Quit
    menu_add_action(main_menu, L"Quit", &QCoreApplication::quit);
    
    return main_menu;
}

void MainWindow::open_tab0()
{
    mainwindowTab->setCurrentIndex(0);
    this->MyShow();
}
void MainWindow::open_tab1()
{
    mainwindowTab->setCurrentIndex(1);
    this->MyShow();
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


// ====================== mainwindow_TrayIcon End ======================
