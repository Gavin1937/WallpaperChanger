#pragma once

// Qt Libs
#include <QApplication>
#include <QCheckBox>
#include <QCloseEvent>
#include <QComboBox>
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
#include <algorithm>
#include <ctime>
#include <fstream>
#include <stdexcept>
#include <string>

// Windows API
#include <Shlobj.h>
#include <shtypes.h>
#include <windows.h>

// others
#include "CacheBrowserDlg.h"
#include "ImageEditorDlg.h"
#include "timer.h"

// utilities
#include "../utilities/AppLaunchHandler.h"
#include "../utilities/CacheRW.h"
#include "../utilities/ConfigManager.h"
#include "../utilities/GlobTools.h"


// class declaration
class MainWindow;

// structs, enums, & global variable
enum class ProgramCloseMode {
    UNKNOWN = 0, EXIT, HIDE
};
enum class DropDownState {
    Seconds = 0, Minutes, Hours, Days, Weeks
};
namespace {
    static bool pub_WhetherRestartAfterCrash = false;
};

// public Recovery Function
DWORD WINAPI RecoveryFunc(PVOID param);


class MainWindow : public QMainWindow, public Ui_MainWindow
{
    Q_OBJECT


// * mainwindow primary members
public:
    // constructor
    MainWindow(QWidget *parent = 0);
    // destructor
    ~MainWindow();
    
    void write_default_config();
    void update_wallpapers();
    
    // ::show() w/ resetting controls
    void MyShow();
    
public slots: // handle universal buttons (OK, Cancel, Apply)
    void onOKPressed();
    void onCancelPressed();
    void onApplyPressed();
    
protected: // protected event handlers
    // on program close
    virtual void closeEvent(QCloseEvent* event);
    
    // custom event
    virtual void customEvent(QEvent* e);
    
    // handle screen resize event
    virtual void screenModeChangeEvent(ScreenModeChangeEvent* e);
    
private: // helper functions
    
    std::wstring get_windows_sys_theme_dir();
    std::pair<int, int> get_physical_screen_res();
    void set_default_wallpaper();
    std::wstring get_default_wallpaper_src();
    
    void paste_default_wallpaper_to_themes();
    
private:
    WallpaperUpdater* p_Wallpaper_Updater;
    ConfigManager m_Config;
    ProgramCloseMode m_ProgramCloseMode; 
    // control status
    bool m_ControlChanged;


// * TrayIcon relate members
public slots: // TrayIcon slots
    void iconActivated(QSystemTrayIcon::ActivationReason);
    
private: // helper functions
    void init_SysTrayIcon(QIcon* icon);
    QMenu* createMenu();
    
    void open_tab0();
    void open_tab1();
    
    template<class FUNC>
    void menu_add_action(QMenu* menu, const std::wstring& action_name, FUNC action_func);
    QMenu* menu_add_menu(QMenu* parent_menu, const std::wstring& child_menu_name, QMenu* child_menu = nullptr);
    
private:
    QSystemTrayIcon* p_TrayIcon;
    QMenu* p_TrayIconMenu;



// * Wallpaper Tab relate members
public:
    bool is_all_wallpaper_set();
    
public slots: // Wallpaper Tab slots
    void onTab0_TextEditChanged();
    void onBrowseCacheBntPressed();
    
protected: // event handling functions
    // custom event handler
    virtual void addFileFromComputerEvent(AddFileFromComputerEvent* event);
    virtual void addFileFromCacheEvent(AddFileFromCacheEvent* event);
    virtual void removeCacheEvent(RemoveCacheEvent* event);
    virtual void editCacheEvent(EditCacheEvent* event);
    virtual void cacheInfoEvent(CacheInfoEvent* event);
    
private: // helper functions
    void init_WallpaperTab();
    void WallpaperTab_resetCtrls();
    void WallpaperTab_makeConnections();
    void save_WallpaperTab();
    
    // adding wallpapers
    QString add_default_wallpaper();
    void select_default_wallpaper();
    QString add_landscape_wallpaper();
    void select_landscape_wallpaper();
    QString add_portrait_wallpaper();
    void select_portrait_wallpaper();
    QString select_image(std::string dlg_caption = "Select Image", std::string default_filename = "/Select Image");
    QString add_image_as_wallpaper(const QString& file_path);
    bool is_cached_file(const QString& file_path, QString& wallpaperID_output);
    bool is_cached_file(const QString& file_path);
    
    // helper function
    QString get_wallpaper_src(const QString& wallpaper_id);
    
private:
    // last select path
    QString m_LastSelectPath;
    // Wallpaper Id
    QString m_DefaultWallpaper;
    QString m_LandscapeWallpaper;
    QString m_PortraitWallpaper;
    // CacheBrowserDlg
    CacheBrowserDlg *p_CacheBrowserDlg;
    // Deletion Item Buffer for p_CacheBrowserDlg
    QVector<QString> m_DeleteBuff;
    // ImageEditorDlg
    ImageEditorDlg *p_ImageEditorDlg;



// * Setting Tab relate members
public slots: // Setting Tab slots
    void onTab1_TextEditChanged();
    void onTab1_DropDownChanged();
    void onTab1_ChkBoxStatusChanged_HideWhenClosed();
    void onTab1_ChkBoxStatusChanged_LaunchAtStartup();
    void onTab1_ChkBoxStatusChanged_RestartAfterCrash();
    
private: // helper functions
    void init_SettingTab();
    void SettingTab_resetCtrls();
    void SettingTab_makeConnections();
    void save_SettingTab();
    
    // convert m_WallpaperUpdateInterval between different DropDownState (sec, min, hour, day, week)
    void time_converter(int& time, const DropDownState& curr_state, const DropDownState& last_state);
    
private:
    DropDownState m_DropDownState;
    int m_WallpaperUpdateInterval;


};
