
// others
#include "mainwindow.h"


// ====================== Public Function ======================

// public Recovery Function
DWORD WINAPI RecoveryFunc(PVOID param)
{
    HRESULT hres = S_OK;
    BOOL userCanceled = FALSE;
    std::ofstream out;
    time_t rawtime = 0;
    std::string msg;
    
    // Notify Windows for RecoveryFunc
    hres = ApplicationRecoveryInProgress(&userCanceled);
    if (FAILED(hres) || userCanceled)
        goto cleanup;
    
    // log current time
    rawtime = time(0);
    msg = asctime(localtime(&rawtime));
    msg = "Crash At: " + msg;
    out.open(GlobTools::getCurrExePathA() + "CRASHLOG.log", std::ios::app);
    if (out.fail())
        goto cleanup;
    out << msg;
    
    // Register for Application Restart if needed
    if (pub_WhetherRestartAfterCrash) {
        hres = RegisterApplicationRestart(0, RESTART_NO_PATCH | RESTART_NO_REBOOT);
        if (FAILED(hres))
            goto cleanup;
    }
    
    // cleanup
    goto cleanup;
cleanup:
    out.close();
    ApplicationRecoveryFinished((userCanceled) ? FALSE: TRUE);
    return 0;
}


// ====================== Public Function End ======================





// ====================== MainWindow ======================

// public

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_Config(),
    p_Wallpaper_Updater(nullptr),
    m_ProgramCloseMode(ProgramCloseMode::UNKNOWN),
    m_ControlChanged(false),
// SysTrayIcon relate
    p_TrayIcon(new QSystemTrayIcon(this)),
    p_TrayIconMenu(nullptr),
// WallpaperTab relate
    m_LastSelectPath(""),
    m_DefaultWallpaper(""),
    m_LandscapeWallpaper(""),
    m_PortraitWallpaper(""),
    p_CacheBrowserDlg(nullptr),
    m_DeleteBuff(QVector<QString>()),
    p_ImageEditorDlg(nullptr),
// SettingTab relate
    m_DropDownState(DropDownState::Seconds),
    m_WallpaperUpdateInterval(0)
{
    // App icon
    QIcon appIcon = QIcon(":/res/icon.png");
    this->setWindowIcon(appIcon);
    // init System Tray Icon
    init_SysTrayIcon(&appIcon);
    
    // initialize config
    std::wstring config_path = GlobTools::getCurrExePathW();
    config_path += L"config.ini";
    try {
        m_Config.read(config_path);
    } catch(std::exception& err) { // cannot find config.ini under current dir
        // write a new config.ini
        write_default_config();
    }
    
    // check whether config.ini has all basic element set
    bool is_config_good = false;
    try {
        is_config_good = 
            !m_Config.get(L"system", L"windows_theme_dir").empty() &&
            !m_Config.get(L"system", L"windows_startup_dir").empty() &&
            !m_Config.get(L"program", L"core_program").empty() &&
            !m_Config.get(L"program", L"gui_program").empty()
        ;
    } catch(...) {
        is_config_good = false;
    }
    // config.ini does not have all basic element set
    // clear all cache & user setting
    if (!is_config_good) { 
        handle_bad_config_wallpaperlists();
    }
    
    // check whether to update wallpapers
    bool has_all_three_wallpaper_set = 
        m_Config.get(L"wallpaper", L"landscape_wallpaper_id").empty() ||
        m_Config.get(L"wallpaper", L"portrait_wallpaper_id").empty() ||
        m_Config.get(L"wallpaper", L"default_wallpaper_id").empty();
    if (!has_all_three_wallpaper_set) { // no empty wallpaper, have all landscape, portrait, & default
        try {
            // set default wappaper and update wallpaper
            set_default_wallpaper();
        } catch(...) {
            handle_bad_config_wallpaperlists();
        }
    }
    
    // setup UI
    setupUi(this);
    setFixedSize(420,510);
    mainwindowTab->setCurrentIndex(0);
    // init "Wallpaper" tab (index: 0)
    init_WallpaperTab();
    // init "Setting" tab (index: 1)
    init_SettingTab();
    
    // init timer
    p_Wallpaper_Updater = new WallpaperUpdater();
}
MainWindow::~MainWindow()
{
    // deallocate memories
    delete p_Wallpaper_Updater;
}

void MainWindow::write_default_config()
{
    // system
    m_Config.add(L"system", L"windows_theme_dir", get_windows_sys_theme_dir());
    m_Config.add(L"system", L"windows_startup_dir", get_windows_sys_startup_dir());
    m_Config.add(L"system", L"launch_at_startup", L"false");
    m_Config.add(L"system", L"restart_after_crash", L"false");
    auto res = get_physical_screen_res();
    m_Config.add(L"system", L"horz_res", std::to_wstring(res.first));
    m_Config.add(L"system", L"vert_res", std::to_wstring(res.second));
    
    // program
    m_Config.add(L"program", L"core_program", GlobTools::getCurrExePathW() + L"WallpaperChanger-core.exe");
    m_Config.add(L"program", L"gui_program", GlobTools::getCurrExePathW() + L"WallpaperChanger-gui.exe");
    m_Config.add(L"program", L"hide_when_closed", L"true"); // toggle hide/exit when user close program
    m_Config.add(L"program", L"wallpaper_update_time", L"1800"); // val in seconds, default = 30min
    
    // wallpaper
    m_Config.add(L"wallpaper", L"default_wallpaper_id", L"");
    m_Config.add(L"wallpaper", L"landscape_wallpaper_id", L"");
    m_Config.add(L"wallpaper", L"portrait_wallpaper_id", L"");
    
    // write config file
    m_Config.write(GlobTools::getCurrExePathW()+L"config.ini");
}

void MainWindow::update_wallpapers()
{
    // replace ./TranscodedWallpaper w/ default wallpaper
    try {
        paste_default_wallpaper_to_themes();
    } catch (std::exception& err) {
        throw err;
    }
    
    // rm -rf /CachedFiles/ first, and then re-create that dir 
    if (getScreenMode() == ScreenMode::Single) { // if is single screen
        // get dirs
        QDir theme_dir(QString::fromWCharArray(m_Config.get(L"system", L"windows_theme_dir").c_str()));
        QDir cache_dir(theme_dir.absolutePath()+"/CachedFiles");
        // remove auto create dir and make a new one
        cache_dir.removeRecursively(); 
        theme_dir.mkdir("CachedFiles");
    }
    
    // update landscape wallpaper
    QObject parent1;
    QString program1 = QString::fromWCharArray(m_Config.get(L"program", L"core_program").c_str());
    QStringList arguments1;
    arguments1
            << "--paste"
            << QString::fromWCharArray(m_Config.get(L"wallpaper", L"landscape_wallpaper_id").c_str())
            << "LANDSCAPE"
    ;
    QProcess myProcess1(&parent1);
    myProcess1.start(program1, arguments1);
    // wait for myProcess1 to finish 
    while (!myProcess1.waitForFinished())
        Sleep(500);
    myProcess1.close();
    
    // update portrait wallpaper
    QObject parent2;
    QString program2 = QString::fromWCharArray(m_Config.get(L"program", L"core_program").c_str());
    QStringList arguments2;
    arguments2
            << "--paste"
            << QString::fromWCharArray(m_Config.get(L"wallpaper", L"portrait_wallpaper_id").c_str())
            << "PORTRAIT"
    ;
    QProcess myProcess2(&parent2);
    myProcess2.start(program2, arguments2);
    // wait for myProcess2 to finish
    while (!myProcess2.waitForFinished())
        Sleep(500);
    myProcess2.close();
    
    // clean ./core_cache file
    CleanCache(GlobTools::getCurrExePathW()+L"core_cache");
}

// ::show() w/ resetting controls
void MainWindow::MyShow()
{
    // reset controls
    WallpaperTab_resetCtrls();
    SettingTab_resetCtrls();
    // show MainWindow
    this->show();
}


// public slots: handle bnt OK, Cancel, & Apply through whole program

// save all setting & quit App dlg
void MainWindow::onOKPressed()
{
    // save
    if (m_ControlChanged) {
        save_WallpaperTab();
        save_SettingTab();
        m_ControlChanged = false;
    }
    // quit
    if (is_all_wallpaper_set() && !m_ControlChanged)
        this->hide();
    else {
        std::wstring msg(
            L"There are uncompleted Wallpaper Information or unsaved changes.\n"
            L"Please fill in all the Wallpaper Information and save changes with \"OK\" or \"Apply\"."
        );
        MessageBoxW(0, msg.c_str(), L"Warning", 0);
        return;
    }
}
// quit App dlg
void MainWindow::onCancelPressed()
{
    // quit
    if (is_all_wallpaper_set() && !m_ControlChanged)
        this->hide();
    else {
        std::wstring msg(
            L"There are uncompleted Wallpaper Information or unsaved changes.\n"
            L"Please fill in all the Wallpaper Information and save changes with \"OK\" or \"Apply\"."
        );
        MessageBoxW(0, msg.c_str(), L"Warning", 0);
        return;
    }
}
// save all setting
void MainWindow::onApplyPressed()
{
    // save
    if (m_ControlChanged) {
        save_WallpaperTab();
        save_SettingTab();
        m_ControlChanged = false;
    }
}
// Launch About Program Dialog
void MainWindow::AboutProgramDlg()
{
    QString msg = 
        "Program Version: " + QString::number(PROGRAM_VERSION) + "<br>" +
        QString("Program Author: Gavin1937<br>") +
        QString("Github: ") + 
        QString("<a href=\"https://github.com/Gavin1937/WallpaperChanger\">https://github.com/Gavin1937/WallpaperChanger</a><br>")
    ;
    QApplication::setQuitOnLastWindowClosed(false);
    QMessageBox::about(this, "About WallpaperChanger", msg);
}

// clear cached wallpapers
void MainWindow::onClearCachedWallpapers()
{
    clear_wallpaper_cache();
    m_Config.set(L"wallpaper", L"default_wallpaper_id", L"");
    m_Config.set(L"wallpaper", L"landscape_wallpaper_id", L"");
    m_Config.set(L"wallpaper", L"portrait_wallpaper_id", L"");
    WallpaperTab_resetCtrls();
}


// protected event handlers

// on program close
void MainWindow::closeEvent(QCloseEvent* event)
{
    // handle unfinished wallpaper path info or unsaved changes
    if (!is_all_wallpaper_set() || m_ControlChanged) {
        std::wstring msg(
            L"There are uncompleted Wallpaper Information or unsaved changes.\n"
            L"Please fill in all the Wallpaper Information and save changes with \"OK\" or \"Apply\"."
        );
        MessageBoxW(0, msg.c_str(), L"Warning", 0);
        event->ignore();
        return;
    } else {
        switch(m_ProgramCloseMode)
        {
        case ProgramCloseMode::EXIT:
            qApp->exit();
            break;
        default: // HIDE & UNKNOWN
            event->ignore();
            this->hide();
            break;
        }
    }
}

// custom event
void MainWindow::customEvent(QEvent* e)
{
    if (ScreenModeChangeEvent* SMC_event = dynamic_cast<ScreenModeChangeEvent*>(e))
        screenModeChangeEvent(SMC_event);
    else if (AddFileFromComputerEvent* AFFC1_event = dynamic_cast<AddFileFromComputerEvent*>(e))
        addFileFromComputerEvent(AFFC1_event);
    else if (AddFileFromCacheEvent* AFFC2_event = dynamic_cast<AddFileFromCacheEvent*>(e))
        addFileFromCacheEvent(AFFC2_event);
    else if (RemoveCacheEvent* RC_event = dynamic_cast<RemoveCacheEvent*>(e))
        removeCacheEvent(RC_event);
    else if (EditCacheEvent* EC_event = dynamic_cast<EditCacheEvent*>(e))
        editCacheEvent(EC_event);
    else if (CacheInfoEvent* CI_event = dynamic_cast<CacheInfoEvent*>(e))
        cacheInfoEvent(CI_event);
}

// handle screen resize event
void MainWindow::screenModeChangeEvent(ScreenModeChangeEvent* e)
{
    update_wallpapers();
}


// private:

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
std::wstring MainWindow::get_windows_sys_startup_dir()
{
    std::wstring output = QDir::homePath().toStdWString();
    size_t temp = output.find(L'/');
    while (temp != std::wstring::npos) {
        output.replace(output.begin()+temp, output.begin()+temp+1, L"\\");
        temp = output.find(L'/');
    }
    output += L"\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\";
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
    std::wstring default_wallpaper_id = m_Config.get(L"wallpaper", L"default_wallpaper_id");
    std::wstring default_wallpaper_src = get_default_wallpaper_src();
    if (!default_wallpaper_src.empty()) {
        QString path(QString::fromWCharArray(default_wallpaper_src.c_str()));
        // set registry wallpaper path value
        QSettings settings("HKEY_CURRENT_USER\\Control Panel\\Desktop",
                                QSettings::NativeFormat);
        settings.setValue("Wallpaper", QVariant(path));
        // replace ./TranscodedWallpaper w/ default wallpaper
        try {
            paste_default_wallpaper_to_themes();
        } catch (std::exception& err) {
            throw err;
        }
        
        // set wallpaper
        if (SystemParametersInfoW(SPI_SETDESKWALLPAPER, 1, default_wallpaper_src.data(),
            SPIF_SENDWININICHANGE) != 0)
        { //success
            // SystemParametersInfoW() will delete /CachedFiles/ under Windows Theme Dir
            // So update /CachedFiles/ after success
            update_wallpapers();
        }
    } else {
        throw std::invalid_argument("Cannot set Default Wallpaper");
    }
}
std::wstring MainWindow::get_default_wallpaper_src()
{
    std::wstring default_wallpaper_id = m_Config.get(L"wallpaper", L"default_wallpaper_id");
    // get default wallpaper info
    QObject parent;
    QString program = QString::fromWCharArray(m_Config.get(L"program", L"core_program").c_str());
    QStringList arguments;
    arguments
            << "--find"
            << QString::fromStdWString(default_wallpaper_id)
    ;
    QProcess myProcess(&parent);
    myProcess.start(program, arguments);
    // get wallpaper ID from ./core_cache
    while (!myProcess.waitForFinished())
        Sleep(500);
    myProcess.close();
    // read new added wallpaper from cache 
    Cache_ReaderW cache(GlobTools::getCurrExePathW()+L"core_cache");
    if (cache.isCacheExist()) {
        std::wstring output(cache.getData()->at(0));
        output.assign(output.begin()+output.find(L':')+2, output.end());
        return output;
    } else {
        return std::wstring();
    }
}
void MainWindow::paste_default_wallpaper_to_themes()
{
    std::wstring default_wallpaper_id = m_Config.get(L"wallpaper", L"default_wallpaper_id");
    // replace ./TranscodedWallpaper w/ default wallpaper
    QObject parent;
    QString program = QString::fromWCharArray(m_Config.get(L"program", L"core_program").c_str());
    QStringList arguments;
    arguments
        << "--paste"
        << QString::fromStdWString(default_wallpaper_id)
        << "DEFAULT"
        ;
    QProcess myProcess(&parent);
    myProcess.start(program, arguments);
    // wait for core to finish
    while (!myProcess.waitForFinished())
        Sleep(500);
    myProcess.close();
    
    // read new added wallpaper from cache 
    Cache_ReaderW cache(GlobTools::getCurrExePathW()+L"core_cache");
    if (cache.isCacheExist() && cache.getData()->at(0) == L"1") {
        if (cache.getData()->at(1) != default_wallpaper_id)
            throw std::invalid_argument("Error occurs during replacing TranscodedWallpaper");
    }
    else {
        throw std::invalid_argument("Error occurs during reading/writing core_cache file");
    }
}

void MainWindow::clear_wallpaper_cache()
{
    QObject parent;
    QString program = QString::fromWCharArray(m_Config.get(L"program", L"core_program").c_str());
    QStringList arguments;
    arguments
        << "--clear"
        << "CACHED_WALLPAPER"
        ;
    QProcess myProcess(&parent);
    myProcess.start(program, arguments);
    // wait for core to finish
    while (!myProcess.waitForFinished())
        Sleep(500);
    myProcess.close();
}
void MainWindow::clear_user_config()
{
    QObject parent;
    QString program = QString::fromWCharArray(m_Config.get(L"program", L"core_program").c_str());
    m_Config.~ConfigManager();
    QStringList arguments;
    arguments
        << "--clear"
        << "USER_CONFIG"
        ;
    QProcess myProcess(&parent);
    myProcess.start(program, arguments);
    // wait for core to finish
    while (!myProcess.waitForFinished())
        Sleep(500);
    myProcess.close();
}
void MainWindow::clear_all()
{
    QObject parent;
    QString program = QString::fromWCharArray(m_Config.get(L"program", L"core_program").c_str());
    m_Config.~ConfigManager();
    QStringList arguments;
    arguments
        << "--clear"
        << "ALL"
        ;
    QProcess myProcess(&parent);
    myProcess.start(program, arguments);
    // wait for core to finish
    while (!myProcess.waitForFinished())
        Sleep(500);
    myProcess.close();
}
void MainWindow::handle_bad_config_wallpaperlists()
{
    std::wstring msg = 
        L"The \"config.ini\" or \"Wallpapers/WallpaperList\" under current directory is corrupted.\n"
        L"In order to reset, program will remove \"config.ini\" and Exit.\n"
        L"\n"
        L"Do you also want to remove all cached Wallpapers under \"Wallpapers\"?\n"
        L"Press \"Yes\" to remove \"config.ini\" and all cached Wallpapers.\n"
        L"Press \"No\" to only remove \"config.ini\".\n"
        L"Press \"Cancel\" to Cancel this process.\n"
    ;
    int msg_res = MessageBoxW(0, msg.c_str(), L"Error!", MB_YESNOCANCEL);
    switch (msg_res)
    {
    case IDYES:
        clear_all();
        break;
    case IDNO:
        clear_user_config();
        break;
    case IDCANCEL:
        break;
    }
    qApp->exit(EXIT_FAILURE);
}


// ====================== MainWindow End ======================