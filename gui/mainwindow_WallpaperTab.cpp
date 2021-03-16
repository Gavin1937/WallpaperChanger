
// Wallpaper Tab UI implementation of class MainWindow

#include "mainwindow.h"


// ====================== mainwindow_WallpaperTab ======================

// public:
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


// Wallpaper Tab slots
void MainWindow::onTab0_TextEditChanged()
{
    m_ControlChanged = true;
}
void MainWindow::onBrowseCacheBntPressed()
{
    p_CacheBrowserDlg = new CacheBrowserDlg(&m_Config, this);
    p_CacheBrowserDlg->exec();
    delete p_CacheBrowserDlg;
}


// protected: // event handling functions

// custom event handler
void MainWindow::addFileFromComputerEvent(AddFileFromComputerEvent* event)
{
    switch(event->m_ItemSection)
    {
    case ItemSections::DefaultWallpaper:
        select_default_wallpaper();
        add_default_wallpaper();
        break;
    case ItemSections::LandscapeWallpaper:
        select_landscape_wallpaper();
        add_landscape_wallpaper();
        break;
    case ItemSections::PortraitWallpaper:
        select_portrait_wallpaper();
        add_portrait_wallpaper();
        break;
    case ItemSections::OthersWallpaper: case ItemSections::Unknown:
        add_image_as_wallpaper(select_image());
        break;
    }
    QApplication::instance()->postEvent(
        event->p_EventSource,
        new ReloadWallpapersEvent()
    );
}
void MainWindow::addFileFromCacheEvent(AddFileFromCacheEvent* event)
{
    
}
void MainWindow::removeCacheEvent(RemoveCacheEvent* event)
{
    
}
void MainWindow::editCacheEvent(EditCacheEvent* event)
{
    
}
void MainWindow::cacheInfoEvent(CacheInfoEvent* event)
{
    
}



// private:
void MainWindow::init_WallpaperTab()
{
    WallpaperTab_resetCtrls();
    WallpaperTab_makeConnections();
}
void MainWindow::WallpaperTab_resetCtrls()
{
    // setup "Wallpaper" tab
    mainwindowTab->setTabText(0, "Wallpaper");
    // set TextEdit text
    is_all_wallpaper_set();
}
void MainWindow::WallpaperTab_makeConnections()
{
    // connect controls to functions
    connect(defaultBrowseBnt, &QPushButton::clicked, this, &MainWindow::select_default_wallpaper);
    connect(landscapeBrowseBnt, &QPushButton::clicked, this, &MainWindow::select_landscape_wallpaper);
    connect(portraitBrowseBnt, &QPushButton::clicked, this, &MainWindow::select_portrait_wallpaper);
    connect(defaultTextEdit, &QLineEdit::textChanged, this, &MainWindow::onTab0_TextEditChanged);
    connect(landscapeTextEdit, &QLineEdit::textChanged, this, &MainWindow::onTab0_TextEditChanged);
    connect(portraitTextEdit, &QLineEdit::textChanged, this, &MainWindow::onTab0_TextEditChanged);
    connect(defaultCacheBnt, &QPushButton::clicked, this, &MainWindow::onBrowseCacheBntPressed);
    connect(landscapeCacheBnt, &QPushButton::clicked, this, &MainWindow::onBrowseCacheBntPressed);
    connect(portraitCacheBnt, &QPushButton::clicked, this, &MainWindow::onBrowseCacheBntPressed);
    // universal buttons (OK, Cancel, Apply)
    connect(tab0OK, &QPushButton::clicked, this, &MainWindow::onOKPressed);
    connect(tab0Cancel, &QPushButton::clicked, this, &MainWindow::onCancelPressed);
    connect(tab0Apply, &QPushButton::clicked, this, &MainWindow::onApplyPressed);
}
void MainWindow::save_WallpaperTab()
{
    if (!landscapeTextEdit->text().isEmpty()) { // text edit has value
        QString temp_str;
        if (!is_cached_file(landscapeTextEdit->text(), temp_str))
        {
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
        } else if (!temp_str.isEmpty()) { // is cached file
            m_LandscapeWallpaper = temp_str;
            m_Config.set(L"wallpaper", L"landscape_wallpaper_id", m_LandscapeWallpaper.toStdWString());
        }
    }
    if (!portraitTextEdit->text().isEmpty()) { // text edit has value
        QString temp_str;
        if (!is_cached_file(portraitTextEdit->text(), temp_str))
        {
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
        } else if (!temp_str.isEmpty()) { // is cached file
            m_PortraitWallpaper = temp_str;
            m_Config.set(L"wallpaper", L"portrait_wallpaper_id", m_PortraitWallpaper.toStdWString());
        }
    }
    if (!defaultTextEdit->text().isEmpty()) { // text edit has value
        QString temp_str;
        if (!is_cached_file(defaultTextEdit->text(), temp_str))
        {
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
        } else if (!temp_str.isEmpty()) { // is cached file
            m_DefaultWallpaper = temp_str;
            m_Config.set(L"wallpaper", L"default_wallpaper_id", m_DefaultWallpaper.toStdWString());
        }
    }
}

// adding wallpapers
void MainWindow::add_default_wallpaper()
{
    QString temp_id;
    if (m_DefaultWallpaper.isEmpty())
        return;
    else if (is_cached_file(m_DefaultWallpaper, temp_id)) {
        m_Config.set(L"wallpaper", L"default_wallpaper_id", temp_id.toStdWString());
        return;
    }
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
    QString temp_id;
    if (m_LandscapeWallpaper.isEmpty())
        return;
    else if (is_cached_file(m_LandscapeWallpaper, temp_id)) {
        m_Config.set(L"wallpaper", L"landscape_wallpaper_id", temp_id.toStdWString());
        return;
    }
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
    QString temp_id;
    if (m_PortraitWallpaper.isEmpty())
        return;
    else if (is_cached_file(m_PortraitWallpaper, temp_id)) {
        m_Config.set(L"wallpaper", L"portrait_wallpaper_id", temp_id.toStdWString());
        return;
    }
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
    // set current dir
    QString dir;
    if (!m_LastSelectPath.isEmpty())
        dir = m_LastSelectPath + QString(default_filename.c_str());
    else
        dir = QString(default_filename.c_str());
	QString output = QFileDialog::getOpenFileName(this, tr(dlg_caption.data()),
                                                    dir,
                                                    tr("All Images (*.png *.jpg *.jpeg *.bmp)\n"
                                                        "JPEG/JPG (*.jpg *.jpeg)\n"
                                                        "PNG (*.png)\n"
                                                        "BMP (*.bmp)\n"
                                                        "All Files (*.*)"));
    if (!output.isEmpty()) {
        for (auto& it : output)
            if (it == '/') it = '\\';
    }
    // update last select path
    m_LastSelectPath = output;
    std::reverse(m_LastSelectPath.begin(), m_LastSelectPath.end());
    auto it = std::find(m_LastSelectPath.begin(), m_LastSelectPath.end(), '\\');
    size_t shrink_size = it - m_LastSelectPath.begin();
    std::reverse(m_LastSelectPath.begin(), m_LastSelectPath.end());
    m_LastSelectPath.resize(m_LastSelectPath.length() - shrink_size);
    // output
    return output;
}
void MainWindow::add_image_as_wallpaper(const QString& file_path)
{
    if (file_path.isEmpty())
        return;
    else if (is_cached_file(file_path))
        return;
    // cache wallpaper
    QObject parent;
    QString program = QString::fromWCharArray(m_Config.get(L"program", L"core_program").c_str());
    QStringList arguments;
    arguments
            << "--add"
            << file_path
    ;
    QProcess myProcess(&parent);
    myProcess.start(program, arguments);
    // wait for core to finish
    while (!myProcess.waitForFinished())
        Sleep(500);
    myProcess.close();
}
bool MainWindow::is_cached_file(const QString& file_path, QString& wallpaperID_output)
{
    if (!GlobTools::is_filedir_existA(GlobTools::getCurrExePathA()+"Wallpapers\\WallpaperList")) {
        wallpaperID_output = QString();
        return false; // ./Wallpapers/WallpaperList does not exist
    }
    GlobTools::utf8_wifstream WallpaperList(GlobTools::getCurrExePathA()+"Wallpapers\\WallpaperList");
    std::wstring buff;
    std::wstring buff2;
    getline(WallpaperList, buff); // skip 1st line
    while (getline(WallpaperList, buff)) {
        size_t pos1 = buff.find('\"')+1;
        size_t pos2 = buff.find('\"', pos1);
        buff2.assign(buff.begin()+pos1, buff.begin()+pos2);
        if (buff2 == file_path.toStdWString()) {
            pos1 = pos2+3;
            pos2 = pos1+6;
            wallpaperID_output = QString::fromWCharArray(std::wstring(buff.begin()+pos1, buff.begin()+pos2).c_str());
            return true;
        }
    }
    wallpaperID_output = QString();
    return false;
}
bool MainWindow::is_cached_file(const QString& file_path)
{
    if (!GlobTools::is_filedir_existA(GlobTools::getCurrExePathA()+"Wallpapers\\WallpaperList")) {
        return false; // ./Wallpapers/WallpaperList does not exist
    }
    GlobTools::utf8_wifstream WallpaperList(GlobTools::getCurrExePathA()+"Wallpapers\\WallpaperList");
    std::wstring buff;
    std::wstring buff2;
    getline(WallpaperList, buff); // skip 1st line
    while (getline(WallpaperList, buff)) {
        size_t pos1 = buff.find('\"')+1;
        size_t pos2 = buff.find('\"', pos1);
        buff2.assign(buff.begin()+pos1, buff.begin()+pos2);
        if (buff2 == file_path.toStdWString()) {
            pos1 = pos2+3;
            pos2 = pos1+6;
            return true;
        }
    }
    return false;
}

// helper function
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


// ====================== mainwindow_WallpaperTab End ======================
