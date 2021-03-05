
// others
#include "mainwindow.h"


// ====================== MainWindow ======================

// public

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_Config(),
    m_Wallpaper_Updater(nullptr),
    m_ProgramCloseMode(ProgramCloseMode::UNKNOWN),
    m_ControlChanged(false),
// SysTrayIcon relate
    m_TrayIcon(new QSystemTrayIcon(this)),
    m_TrayIconMenu(nullptr),
// WallpaperTab relate
    m_LastSelectPath(""),
    m_DefaultWallpaper(""),
    m_LandscapeWallpaper(""),
    m_PortraitWallpaper(""),
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
    
    // check whether to update wallpapers
    bool is_land_port_wallpaper_empty = 
        m_Config.get(L"wallpaper", L"landscape_wallpaper_id").empty() ||
        m_Config.get(L"wallpaper", L"portrait_wallpaper_id").empty();
    if (!is_land_port_wallpaper_empty) { // no empty wallpaper, have both landscape & portrait
        // set default wappaper and update wallpaper
        set_default_wallpaper();
    }
    
    // setup UI
    setupUi(this);
    setFixedSize(420,430);
    mainwindowTab->setCurrentIndex(0);
    // init "Wallpaper" tab (index: 0)
    init_WallpaperTab();
    // init "Setting" tab (index: 1)
    init_SettingTab();
    
    // init timer
    m_Wallpaper_Updater = new WallpaperUpdater();
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
    CleanCache(L"core_cache");
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
    if (is_all_wallpaper_set())
        this->hide();
    else {
        MessageBoxW(0, L"Please fill in all Wallpapers", L"Warning", 0);
        return;
    }
}
// quit App dlg
void MainWindow::onCancelPressed()
{
    // quit
    if (is_all_wallpaper_set())
        this->hide();
    else {
        MessageBoxW(0, L"Please fill in all Wallpapers", L"Warning", 0);
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


// protected event handlers

// on program close
void MainWindow::closeEvent(QCloseEvent* event)
{
    m_ControlChanged = false;
    if (!is_all_wallpaper_set()) { // handle unfinished wallpaper path info
        MessageBoxW(0, L"Please fill in all Wallpapers", L"Warning", 0);
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

// handle screen resize event
void MainWindow::customEvent(QEvent* e)
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
    // QProcess *myProcess = new QProcess(this);
    myProcess.start(program, arguments);
    // get wallpaper ID from ./core_cache
    while (!myProcess.waitForFinished())
        Sleep(500);
    myProcess.close();
    // read new added wallpaper from cache 
    Cache_ReaderW cache(L"core_cache");
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
	Cache_ReaderW cache(L"core_cache");
	if (cache.isCacheExist()) {
		if (cache.getData()->at(0) != default_wallpaper_id)
			throw std::invalid_argument("Error occurs during replacing TranscodedWallpaper");
	}
	else {
        throw std::invalid_argument("Error occurs during reading/writing core_cache file");
	}
}


// ====================== MainWindow End ======================