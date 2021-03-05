#pragma once

// Qt Libs
#include <QApplication>
#include <QDir>
#include <QEvent>
#include <QFileDialog>
#include <QIcon>
#include <QMainWindow>
#include <QMenu>
#include <QProcess>
#include <QSettings>
#include <QString>
#include <QSystemTrayIcon>
#include <QtPlugin>

// Qt ui
#include "./ui/ui_mainwindow.h"

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
#include "../utilities/CacheRW.h"
#include "../utilities/ConfigManager.h"
#include "../utilities/GlobTools.h"


class MainWindow : public QMainWindow, public Ui_MainWindow
{
    Q_OBJECT
    
// * mainwindow primary members
public:
    // constructor
    MainWindow(QWidget *parent = 0);
    
    void write_default_config();
    void update_wallpapers();
    
protected:
    void customEvent(QEvent* e);
    
private:
    
    // helper functions
    std::wstring get_windows_sys_theme_dir();
    std::pair<int, int> get_physical_screen_res();
    void set_default_wallpaper();
    std::wstring get_default_wallpaper_src();
    
    void paste_default_wallpaper_to_themes();
    
private:
    WallpaperUpdater* m_Wallpaper_Updater;
    ConfigManager m_Config;



// * TrayIcon relate members
public slots: // TrayIcon slots
    void iconActivated(QSystemTrayIcon::ActivationReason);
    
private:
    void init_SysTrayIcon(QIcon* icon);
    QMenu* createMenu();
    
    template<class FUNC>
    void menu_add_action(QMenu* menu, const std::wstring& action_name, FUNC action_func);
    QMenu* menu_add_menu(QMenu* parent_menu, const std::wstring& child_menu_name, QMenu* child_menu = nullptr);
    
private:
    QSystemTrayIcon* m_TrayIcon;
    QMenu* m_TrayIconMenu;



// * General Tab relate members
public:
    bool is_all_wallpaper_set();
    
public slots: // General Tab slots
    void onTextEditChanged();
    void onOKPressed();
    void onCancelPressed();
    void onApplyPressed();
    
private:
    void init_GeneralTab();
    
    // adding wallpapers
    void add_default_wallpaper();
    void select_default_wallpaper();
    void add_landscape_wallpaper();
    void select_landscape_wallpaper();
    void add_portrait_wallpaper();
    void select_portrait_wallpaper();
    QString select_image(std::string dlg_caption = "Select Image", std::string default_filename = "/Select Image");
    
    // helper function
    QString get_wallpaper_src(const QString& wallpaper_id);
    
private:
    // Wallpaper Path
    QString m_DefaultWallpaper;
    QString m_LandscapeWallpaper;
    QString m_PortraitWallpaper;
    // config status
    bool m_ControlChanged;


};
