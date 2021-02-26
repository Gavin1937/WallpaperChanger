#pragma once

// Qt Libs
#include <QApplication>
#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QtPlugin>
#include <QDir>
#include <QString>
#include <QProcess>
#include <QSettings>
#include <QEvent>
#include <QFileDialog>

// C++ STL
#include <stdexcept>
#include <string>

#include <fstream>

// Windows API
#include <windows.h>
#include <shtypes.h>
#include <Shlobj.h>

// others
#include "timer.h"
#include "../utilities/GlobTools.h"
#include "../utilities/ConfigManager.h"
#include "../utilities/CacheRW.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    
    void write_default_config();
    void update_wallpapers();
    
public slots:
    void iconActivated(QSystemTrayIcon::ActivationReason);
    
protected:
    void customEvent(QEvent* e);
    
private:
    QMenu* createMenu();
    
    // helper functions
    std::wstring get_windows_sys_theme_dir();
    std::pair<int, int> get_physical_screen_res();
    void set_default_wallpaper();
    std::wstring get_default_wallpaper_src();

    void paste_default_wallpaper_to_themes();
    
    void add_default_wallpaper();
    void add_landscape_wallpaper();
    void add_portrait_wallpaper();
    QString select_image(std::string dlg_caption = "Select Image", std::string default_filename = "/Select Image");
    
    template<class FUNC>
    void menu_add_action(QMenu* menu, const std::wstring& action_name, FUNC action_func);
    QMenu* menu_add_menu(QMenu* parent_menu, const std::wstring& child_menu_name, QMenu* child_menu = nullptr);
    
    
private:
    QSystemTrayIcon* m_TrayIcon;
    QMenu* m_TrayIconMenu;
    WallpaperUpdater* m_Wallpaper_Updater;
    
    ConfigManager m_Config;
};
