#pragma once

// Qt Libs
#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QtPlugin>
#include <QDir>
#include <QString>

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
    
public slots:
    void iconActivated(QSystemTrayIcon::ActivationReason);
    
private:
    QMenu* createMenu();
    
    // helper functions
    std::wstring get_windows_sys_theme_dir();
    std::pair<int, int> get_physical_screen_res();
    
private:
    QSystemTrayIcon* trayIcon;
    QMenu* trayIconMenu;
    Timer timer;
    
    ConfigManager m_Config;
};
