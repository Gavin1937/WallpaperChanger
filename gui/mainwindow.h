#pragma once

// Qt Libs
#include <QApplication>
#include <QCloseEvent>
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
#include <algorithm>

// Windows API
#include <windows.h>
#include <shtypes.h>
#include <Shlobj.h>

// others
#include "timer.h"
#include "../utilities/CacheRW.h"
#include "../utilities/ConfigManager.h"
#include "../utilities/GlobTools.h"

// structs & enums
enum class ProgramCloseMode {
    UNKNOWN = 0, EXIT, HIDE
};


class MainWindow : public QMainWindow, public Ui_MainWindow
{
    Q_OBJECT


// * mainwindow primary members
public:
    // constructor
    MainWindow(QWidget *parent = 0);
    
    void write_default_config();
    void update_wallpapers();
    
public slots: // handle universal buttons (OK, Cancel, Apply)
    void onOKPressed();
    void onCancelPressed();
    void onApplyPressed();
    
protected: // protected event handlers
    // on program close
    virtual void closeEvent(QCloseEvent * event);
    
    // handle screen resize event
    void customEvent(QEvent* e);
    
private: // helper functions
    
    std::wstring get_windows_sys_theme_dir();
    std::pair<int, int> get_physical_screen_res();
    void set_default_wallpaper();
    std::wstring get_default_wallpaper_src();
    
    void paste_default_wallpaper_to_themes();
    
private:
    WallpaperUpdater* m_Wallpaper_Updater;
    ConfigManager m_Config;
    ProgramCloseMode m_ProgramCloseMode; 



// * TrayIcon relate members
public slots: // TrayIcon slots
    void iconActivated(QSystemTrayIcon::ActivationReason);
    
private: // helper functions
    void init_SysTrayIcon(QIcon* icon);
    QMenu* createMenu();
    
    template<class FUNC>
    void menu_add_action(QMenu* menu, const std::wstring& action_name, FUNC action_func);
    QMenu* menu_add_menu(QMenu* parent_menu, const std::wstring& child_menu_name, QMenu* child_menu = nullptr);
    
private:
    QSystemTrayIcon* m_TrayIcon;
    QMenu* m_TrayIconMenu;



// * Wallpaper Tab relate members
public:
    bool is_all_wallpaper_set();
    
public slots: // Wallpaper Tab slots
    void onTextEditChanged();
    
private: // helper functions
    void init_WallpaperTab();
    
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
    // last select path
    QString m_LastSelectPath;
    // Wallpaper Path
    QString m_DefaultWallpaper;
    QString m_LandscapeWallpaper;
    QString m_PortraitWallpaper;
    // config status
    bool m_ControlChanged;



// * Setting Tab relate members
private:
    void init_SettingTab();



};
