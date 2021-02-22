#pragma once

// Qt Libs
#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QtPlugin>
#include <QDir>
#include <QString>
#include <QProcess>

// C++ STL
#include <stdexcept>
#include <windows.h>

// others
#include "timer.h"
#include "GlobTools.h"
#include "ConfigManager.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    
    void write_default_config();
    void update_wallpapers();
    
public slots:
    void iconActivated(QSystemTrayIcon::ActivationReason);
    
private:
    QMenu* createMenu();
    
    // helper functions
    std::wstring get_windows_sys_theme_dir();
    std::pair<int, int> get_physical_screen_res();
    
    template<class FUNC>
    void menu_add_action(QMenu* menu, const std::wstring& action_name, FUNC action_func);
    QMenu* menu_add_menu(QMenu* parent_menu, const std::wstring& child_menu_name, QMenu* child_menu = nullptr);
    
private:
    QSystemTrayIcon* trayIcon;
    QMenu* trayIconMenu;
    Timer timer;
    
    ConfigManager m_Config;
};
