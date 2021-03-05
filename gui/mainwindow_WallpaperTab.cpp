
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
void MainWindow::onTextEditChanged()
{
    m_ControlChanged = true;
}


// private:
void MainWindow::init_WallpaperTab()
{
    // setup "Wallpaper" tab
    mainwindowTab->setTabText(0, "Wallpaper");
    connect(defaultBrowseBnt, &QPushButton::clicked, this, &MainWindow::select_default_wallpaper);
    connect(landscapeBrowseBnt, &QPushButton::clicked, this, &MainWindow::select_landscape_wallpaper);
    connect(portraitBrowseBnt, &QPushButton::clicked, this, &MainWindow::select_portrait_wallpaper);
    connect(defaultTextEdit, &QLineEdit::textChanged, this, &MainWindow::onTextEditChanged);
    connect(landscapeTextEdit, &QLineEdit::textChanged, this, &MainWindow::onTextEditChanged);
    connect(portraitTextEdit, &QLineEdit::textChanged, this, &MainWindow::onTextEditChanged);
    // universal buttons (OK, Cancel, Apply)
    connect(tab0OK, &QPushButton::clicked, this, &MainWindow::onOKPressed);
    connect(tab0Cancel, &QPushButton::clicked, this, &MainWindow::onCancelPressed);
    connect(tab0Apply, &QPushButton::clicked, this, &MainWindow::onApplyPressed);
}

// adding wallpapers
void MainWindow::add_default_wallpaper()
{
    if (m_DefaultWallpaper.isEmpty())
        return;
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
                                                    // tr(default_filename.data()),
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


// ====================== mainwindow_WallpaperTab End ======================
