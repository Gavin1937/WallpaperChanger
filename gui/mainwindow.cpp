
// others
#include "mainwindow.h"


// ====================== MainWindow ======================

// public

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_TrayIcon(new QSystemTrayIcon(this)),
    m_Wallpaper_Updater(nullptr),
    m_TrayIconMenu(nullptr), m_Config(),
    m_ControlChanged(false),
    m_DefaultWallpaper(""),
    m_LandscapeWallpaper(""),
    m_PortraitWallpaper("")
{
    // App icon
    auto appIcon = QIcon(":/res/icon.png");
    this->m_TrayIcon->setIcon(appIcon);
    this->setWindowIcon(appIcon);
    
    // setup UI
    setupUi(this);
    // this->setFixedSize(420,430);
    setFixedSize(420,430);
    // setup "General" tab
    mainwindowTab->setTabText(0, "General");
    connect(defaultBrowseBnt, &QPushButton::clicked, this, &MainWindow::select_default_wallpaper);
    connect(landscapeBrowseBnt, &QPushButton::clicked, this, &MainWindow::select_landscape_wallpaper);
    connect(portraitBrowseBnt, &QPushButton::clicked, this, &MainWindow::select_portrait_wallpaper);
    connect(defaultTextEdit, &QLineEdit::textChanged, this, &MainWindow::onTextEditChanged);
    connect(landscapeTextEdit, &QLineEdit::textChanged, this, &MainWindow::onTextEditChanged);
    connect(portraitTextEdit, &QLineEdit::textChanged, this, &MainWindow::onTextEditChanged);
    connect(tab0OK, &QPushButton::clicked, this, &MainWindow::onOKPressed);
    connect(tab0Cancel, &QPushButton::clicked, this, &MainWindow::onCancelPressed);
    connect(tab0Apply, &QPushButton::clicked, this, &MainWindow::onApplyPressed);
    
    // initialize config
    std::wstring config_path = GlobTools::getCurrExePathW();
    config_path += L"config.ini";
    try {
        m_Config.read(config_path);
    } catch(std::exception& err) { // cannot find config.ini under current dir
        // write a new config.ini
        write_default_config();
        // prompt user to input basic configs
        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        // ! NEED TO REPLACE WITH PROPER DIALOG PROMPT
        // MessageBoxW(0, L"Missing Components in config.ini", L"Warning", 0);
        // return;
        
        
        // // set App dlg text edit
        // if (defaultWal.isEmpty())
        //     defaultTextEdit->setText("Missing Wallpaper");
        // else
        //     defaultTextEdit->setText(defaultWal);
        // if (landscapeWal.isEmpty())
        //     landscapeTextEdit->setText("Missing Wallpaper");
        // else
        //     landscapeTextEdit->setText(landscapeWal);
        // if (portraitWal.isEmpty())
        //     portraitTextEdit->setText("Missing Wallpaper");
        // else
        //     portraitTextEdit->setText(portraitWal);
    }
    
    
    // check whether to update wallpapers
    bool is_land_port_wallpaper_empty = 
        m_Config.get(L"wallpaper", L"landscape_wallpaper_id").empty() ||
        m_Config.get(L"wallpaper", L"portrait_wallpaper_id").empty();
    if (!is_land_port_wallpaper_empty) { // no empty wallpaper, have both landscape & portrait
        // set default wappaper and update wallpaper
        set_default_wallpaper();
    }
    // } else { // have empty wallpaper, prompt user for input
    //     // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //     // ! NEED TO REPLACE WITH PROPER DIALOG PROMPT
    //     MessageBoxW(0, L"Missing Components in config.ini", L"Warning", 0);
    //     return;
        
    //     // // set default wappaper and update wallpaper
    //     // set_default_wallpaper();
    // }
    
    // init timer
    m_Wallpaper_Updater = new WallpaperUpdater();
    
    
    // Tray icon menu
    m_TrayIconMenu = this->createMenu();
    this->m_TrayIcon->setContextMenu(m_TrayIconMenu);
    
    // Displaying the tray icon
    this->m_TrayIcon->show();
    
    // Interaction
    connect(m_TrayIcon, &QSystemTrayIcon::activated, this, &MainWindow::iconActivated);
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

bool MainWindow::is_all_wallpaper_set()
{
    QString defaultWal, landscapeWal, portraitWal;
    
    // set wallpaper path & loop condition
    defaultWal = QString::fromWCharArray(m_Config.get(L"wallpaper", L"default_wallpaper_id").c_str());
    landscapeWal = QString::fromWCharArray(m_Config.get(L"wallpaper", L"landscape_wallpaper_id").c_str());
    portraitWal = QString::fromWCharArray(m_Config.get(L"wallpaper", L"portrait_wallpaper_id").c_str());
    // set App dlg text edit
    if (!defaultWal.isEmpty())
        defaultTextEdit->setText(get_wallpaper_src(defaultWal));
    if (!landscapeWal.isEmpty())
        landscapeTextEdit->setText(get_wallpaper_src(landscapeWal));
    if (!portraitWal.isEmpty())
        portraitTextEdit->setText(get_wallpaper_src(portraitWal));
    // return
    return ( 
        !defaultWal.isEmpty() &&
        !landscapeWal.isEmpty() &&
        !portraitWal.isEmpty()
    );
}


// public slot:
void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason_)
{
    // switch (reason_) {
    // case QSystemTrayIcon::Trigger:
    //     this->trayIcon->showMessage("Hello", "You clicked me!");
    //     break;
    // default:
    //     ;
    // }
}
void MainWindow::onTextEditChanged()
{
    m_ControlChanged = true;
}

// save all setting & quit App dlg
void MainWindow::onOKPressed()
{
    // save
    if (m_ControlChanged) {
        if (!landscapeTextEdit->text().isEmpty() && // text edit has value
            m_Config.get(L"wallpaper", L"landscape_wallpaper_id").empty()) // config file dont have value
        {
            // QDir tempdir(landscapeTextEdit->text());
            if (GlobTools::is_filedir_existW(landscapeTextEdit->text().toStdWString())) {
                m_LandscapeWallpaper = landscapeTextEdit->text();
                add_landscape_wallpaper();
            }
            else {
                MessageBoxW(0, 
                    (L"Entered File Path:\n"+
                    landscapeTextEdit->text().toStdWString()+
                    L"\nDoes Not Exist.").c_str(), 
                    L"Exception", 0);
            }
        }
        if (!portraitTextEdit->text().isEmpty() && // text edit has value
            m_Config.get(L"wallpaper", L"portrait_wallpaper_id").empty()) // config file dont have value
        {
            // QDir tempdir(portraitTextEdit->text());
            if (GlobTools::is_filedir_existW(portraitTextEdit->text().toStdWString())) {
                m_PortraitWallpaper = portraitTextEdit->text();
                add_portrait_wallpaper();
            }
            else {
                MessageBoxW(0, 
                    (L"Entered File Path:\n"+
                    portraitTextEdit->text().toStdWString()+
                    L"\nDoes Not Exist.").c_str(), 
                    L"Exception", 0);
            }
        }
        if (!defaultTextEdit->text().isEmpty() && // text edit has value
            m_Config.get(L"wallpaper", L"default_wallpaper_id").empty()) // config file dont have value
        {
            // QDir tempdir(defaultTextEdit->text());
            if (GlobTools::is_filedir_existW(defaultTextEdit->text().toStdWString())) {
                m_DefaultWallpaper = defaultTextEdit->text();
                add_default_wallpaper();
            }
            else {
                MessageBoxW(0, 
                    (L"Entered File Path:\n"+
                    defaultTextEdit->text().toStdWString()+
                    L"\nDoes Not Exist.").c_str(), 
                    L"Exception", 0);
            }
            try {
                set_default_wallpaper();
            } catch(std::exception& err) {
                MessageBoxA(0, err.what(), "Exception", 0);
            }
        }
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
        if (!landscapeTextEdit->text().isEmpty() && // text edit has value
            m_Config.get(L"wallpaper", L"landscape_wallpaper_id").empty()) // config file dont have value
        {
            // QDir tempdir(landscapeTextEdit->text());
            if (GlobTools::is_filedir_existW(landscapeTextEdit->text().toStdWString())) {
                m_LandscapeWallpaper = landscapeTextEdit->text();
                add_landscape_wallpaper();
            }
            else {
                MessageBoxW(0, 
                    (L"Entered File Path:\n"+
                    landscapeTextEdit->text().toStdWString()+
                    L"\nDoes Not Exist.").c_str(), 
                    L"Exception", 0);
            }
        }
        if (!portraitTextEdit->text().isEmpty() && // text edit has value
            m_Config.get(L"wallpaper", L"portrait_wallpaper_id").empty()) // config file dont have value
        {
            // QDir tempdir(portraitTextEdit->text());
            if (GlobTools::is_filedir_existW(portraitTextEdit->text().toStdWString())) {
                m_PortraitWallpaper = portraitTextEdit->text();
                add_portrait_wallpaper();
            }
            else {
                MessageBoxW(0, 
                    (L"Entered File Path:\n"+
                    portraitTextEdit->text().toStdWString()+
                    L"\nDoes Not Exist.").c_str(), 
                    L"Exception", 0);
            }
        }
        if (!defaultTextEdit->text().isEmpty() && // text edit has value
            m_Config.get(L"wallpaper", L"default_wallpaper_id").empty()) // config file dont have value
        {
            // QDir tempdir(defaultTextEdit->text());
            if (GlobTools::is_filedir_existW(defaultTextEdit->text().toStdWString())) {
                m_DefaultWallpaper = defaultTextEdit->text();
                add_default_wallpaper();
            }
            else {
                MessageBoxW(0, 
                    (L"Entered File Path:\n"+
                    defaultTextEdit->text().toStdWString()+
                    L"\nDoes Not Exist.").c_str(), 
                    L"Exception", 0);
            }
            try {
                set_default_wallpaper();
            } catch(std::exception& err) {
                MessageBoxA(0, err.what(), "Exception", 0);
            }
        }
        m_ControlChanged = false;
    }
}


// protected:

// receive screen resize event
void MainWindow::customEvent(QEvent* e)
{
    update_wallpapers();
}

// private:

QMenu* MainWindow::createMenu()
{
    auto main_menu = new QMenu(this);
    
    // Update Wallpapers
    menu_add_action(main_menu, L"Update Wallpapers", &MainWindow::update_wallpapers);
    
    // Add >
    // auto add_menu = menu_add_menu(main_menu, L"Add");
    //     // Add Landscape Wallpaper
    //     menu_add_action(add_menu, L"Add Default Wallpaper", &MainWindow::add_default_wallpaper);
    //     menu_add_action(add_menu, L"Add Landscape Wallpaper", &MainWindow::add_landscape_wallpaper);
    //     menu_add_action(add_menu, L"Add Portrait Wallpaper", &MainWindow::add_portrait_wallpaper);
    
    // Set Wallpapers
    menu_add_action(main_menu, L"Set Wallpapers", &MainWindow::show);
    
    // Quit
    menu_add_action(main_menu, L"Quit", &QCoreApplication::quit);
    
    return main_menu;
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
QString MainWindow::get_wallpaper_src(const QString& wallpaper_id)
{
    // get default wallpaper info
    QObject parent;
    QString program = QString::fromWCharArray(m_Config.get(L"program", L"core_program").c_str());
    QStringList arguments;
    arguments
            << "--find"
            << wallpaper_id
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
        return QString::fromWCharArray(output.c_str());
    } else {
        return QString();
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

void MainWindow::add_default_wallpaper()
{
    if (m_DefaultWallpaper.isEmpty())
        return;
    //     // get user input default wallpaper path
    //     default_wallpaper = select_image("Select Default Wallpaper");
    //     if (default_wallpaper.isEmpty())
    //         return;
    // } else {
    //     default_wallpaper = wallpaper;
    // }
    // cache wallpaper
    QObject parent;
    QString program = QString::fromWCharArray(m_Config.get(L"program", L"core_program").c_str());
    QStringList arguments;
    arguments
            << "--add"
            << m_DefaultWallpaper
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
        m_Config.set(L"wallpaper", L"default_wallpaper_id", cache.getData()->at(0));
    } else {
        MessageBoxW(0, L"No core_cache for Default", L"Info", 0);
    }
    
    // update control status 
    m_ControlChanged = true;
}
void MainWindow::select_default_wallpaper()
{
    m_DefaultWallpaper = select_image("Select Default Wallpaper");
    if (!m_DefaultWallpaper.isEmpty())
        defaultTextEdit->setText(m_DefaultWallpaper);
    // update control status 
    m_ControlChanged = true;
}
void MainWindow::add_landscape_wallpaper()
{
    if (m_LandscapeWallpaper.isEmpty())
        return;
    // QString landscape_wallpaper;
    // if (wallpaper.isEmpty()) {
    //     // get user input landscape wallpaper path
    //     landscape_wallpaper = select_image("Select Landscape Wallpaper");
    //     if (landscape_wallpaper.isEmpty())
    //         return;
    // } else {
    //     landscape_wallpaper = wallpaper;
    // }
    // cache wallpaper
    QObject parent;
    QString program = QString::fromWCharArray(m_Config.get(L"program", L"core_program").c_str());
    QStringList arguments;
    arguments
            << "--add"
            << m_LandscapeWallpaper
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
        m_Config.set(L"wallpaper", L"landscape_wallpaper_id", cache.getData()->at(0));
    } else {
        MessageBoxW(0, L"No core_cache for Landscape", L"Info", 0);
    }    
    
    // update control status 
    m_ControlChanged = true;
}
void MainWindow::select_landscape_wallpaper()
{
    m_LandscapeWallpaper = select_image("Select Landscape Wallpaper");
    if (!m_LandscapeWallpaper.isEmpty())
        landscapeTextEdit->setText(m_LandscapeWallpaper);
    // update control status 
    m_ControlChanged = true;
}
void MainWindow::add_portrait_wallpaper()
{
    if (m_PortraitWallpaper.isEmpty())
        return;
    // // get user input portrait wallpaper path
    // QString portrait_wallpaper;
    // if (wallpaper.isEmpty()) {
    //     portrait_wallpaper = select_image("Select Portrait Wallpaper");
    //     if (portrait_wallpaper.isEmpty())
    //         return;
    // } else {
    //     portrait_wallpaper = wallpaper;
    // }
    // cache wallpaper
    QObject parent;
    QString program = QString::fromWCharArray(m_Config.get(L"program", L"core_program").c_str());
    QStringList arguments;
    arguments
            << "--add"
            << m_PortraitWallpaper
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
        m_Config.set(L"wallpaper", L"portrait_wallpaper_id", cache.getData()->at(0));
    } else {
        MessageBoxW(0, L"No core_cache for Portrait", L"Info", 0);
    }
    
    // update control status 
    m_ControlChanged = true;
}
void MainWindow::select_portrait_wallpaper()
{
    m_PortraitWallpaper = select_image("Select Portrait Wallpaper");
    if (!m_PortraitWallpaper.isEmpty())
        portraitTextEdit->setText(m_PortraitWallpaper);
    // update control status 
    m_ControlChanged = true;
}

QString MainWindow::select_image(std::string dlg_caption, std::string default_filename)
{
    if (default_filename[0] != '/')
        default_filename = '/' + default_filename;
	QString output =  QFileDialog::getOpenFileName(this, tr(dlg_caption.data()),
                                                    tr(default_filename.data()),
                                                    tr("All Images (*.png *.jpg *.jpeg *.bmp)\n"
                                                        "JPEG/JPG (*.jpg *.jpeg)\n"
                                                        "PNG (*.png)\n"
                                                        "BMP (*.bmp)\n"
                                                        "All Files (*.*)"));
    if (!output.isEmpty()) {
        for (auto& it : output)
            if (it == '/') it = '\\';
    }
    return output;
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


// ====================== MainWindow End ======================