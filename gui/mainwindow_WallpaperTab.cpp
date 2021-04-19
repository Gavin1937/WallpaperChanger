
// Wallpaper Tab UI implementation of class MainWindow

#include "mainwindow.h"


// ====================== mainwindow_WallpaperTab ======================

// public:
bool MainWindow::is_all_wallpaper_set()
{
    // set wallpaper path & loop condition
    m_DefaultWallpaper = QString::fromWCharArray(m_Config.get(L"wallpaper", L"default_wallpaper_id").c_str());
    m_LandscapeWallpaper = QString::fromWCharArray(m_Config.get(L"wallpaper", L"landscape_wallpaper_id").c_str());
    m_PortraitWallpaper = QString::fromWCharArray(m_Config.get(L"wallpaper", L"portrait_wallpaper_id").c_str());
    
    // set App dlg text edit
    if (!m_DefaultWallpaper.isEmpty())
        defaultTextEdit->setText(get_wallpaper_src(m_DefaultWallpaper));
    if (!m_LandscapeWallpaper.isEmpty())
        landscapeTextEdit->setText(get_wallpaper_src(m_LandscapeWallpaper));
    if (!m_PortraitWallpaper.isEmpty())
        portraitTextEdit->setText(get_wallpaper_src(m_PortraitWallpaper));
    
    // return
    return ( 
        !m_DefaultWallpaper.isEmpty() &&
        !m_LandscapeWallpaper.isEmpty() &&
        !m_PortraitWallpaper.isEmpty()
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
    int result = p_CacheBrowserDlg->exec();
    
    if (result == QDialog::Rejected && !m_DeleteBuff.isEmpty()) {
        for (auto it : m_DeleteBuff) {
            // skip already deleted cache file
            if (!get_wallpaper_src(it).isEmpty()) {
                // delete cache file
                QObject parent;
                QString program = QString::fromWCharArray(m_Config.get(L"program", L"core_program").c_str());
                QStringList arguments;
                arguments
                    << "--del"
                    << it
                ;
                QProcess myProcess(&parent);
                myProcess.start(program, arguments);
                // wait for core to finish
                while (!myProcess.waitForFinished())
                    Sleep(500);
                myProcess.close();
                
                CleanCache(L"core_cache");
            }
        }
        m_DeleteBuff.clear();
    } 
    WallpaperTab_resetCtrls();
}


// protected: // event handling functions

// custom event handler
void MainWindow::addFileFromComputerEvent(AddFileFromComputerEvent* event)
{
    QString new_wallpaper_id;
    switch(event->m_ItemSection)
    {
    case ItemSections::DefaultWallpaper:
        select_default_wallpaper();
        new_wallpaper_id = add_default_wallpaper();
        break;
    case ItemSections::LandscapeWallpaper:
        select_landscape_wallpaper();
        new_wallpaper_id = add_landscape_wallpaper();
        break;
    case ItemSections::PortraitWallpaper:
        select_portrait_wallpaper();
        new_wallpaper_id = add_portrait_wallpaper();
        break;
    case ItemSections::OthersWallpaper: case ItemSections::Unknown:
        new_wallpaper_id = add_image_as_wallpaper(select_image());
        break;
    }
    // notify CacheBrowser to update
    QApplication::instance()->postEvent(
        event->p_EventSource,
        new ReloadWallpapersEvent(
            ReloadWallpaperTasks::AddFromComputer,
            reinterpret_cast<void*>(new QString(new_wallpaper_id)))
    );
    qApp->processEvents();
}
void MainWindow::addFileFromCacheEvent(AddFileFromCacheEvent* event)
{
    // validation
    if (event->m_CurrItemID.isEmpty())
        return;
    
    // clear current recorded wallpaper id if needed
    std::wstring loc_opt_name_old = L"";
	std::wstring old_wall_id = L"";
    switch (event->m_CurrItemSection)
    {
    case ItemSections::DefaultWallpaper:
        loc_opt_name_old = L"default_wallpaper_id";
        break;
    case ItemSections::LandscapeWallpaper:
        loc_opt_name_old = L"landscape_wallpaper_id";
        break;
    case ItemSections::PortraitWallpaper:
        loc_opt_name_old = L"portrait_wallpaper_id";
        break;
        
    default: // event->m_CurrItemSection == (ItemSections::Unkown || ItemSections::OthersWallpaper)
        loc_opt_name_old = L"";
        break;
    }
    if (!loc_opt_name_old.empty()) {
        old_wall_id = m_Config.get(L"wallpaper", loc_opt_name_old);
        m_Config.set(L"wallpaper", loc_opt_name_old, L"");
    }
    
    // set new wallpaper id to config.ini
    std::wstring loc_opt_name_new = L"";
    std::wstring new_wall_id = L"";
    switch(event->m_ToItemSection)
    {
    case ItemSections::DefaultWallpaper:
        loc_opt_name_new = L"default_wallpaper_id";
        break;
    case ItemSections::LandscapeWallpaper:
        loc_opt_name_new = L"landscape_wallpaper_id";
        break;
    case ItemSections::PortraitWallpaper:
        loc_opt_name_new = L"portrait_wallpaper_id";
        break;
    
    default: // event->m_ToItemSection == (ItemSections::Unkown || ItemSections::OthersWallpaper)
        return;
        break;
    }
    if (!loc_opt_name_new.empty()) {
        new_wall_id = m_Config.get(L"wallpaper", loc_opt_name_new);
        m_Config.set(L"wallpaper", loc_opt_name_new, event->m_CurrItemID.toStdWString());
    }
    
    // swap 2 wallpapers on Default, Landscape, or Portrait if possible
    if (!old_wall_id.empty() && !new_wall_id.empty())
        m_Config.set(L"wallpaper", loc_opt_name_old, new_wall_id);
    
    // notify CacheBrowser to update
    QApplication::instance()->postEvent(
        event->p_EventSource,
        new ReloadWallpapersEvent()
    );
    qApp->processEvents();
}
void MainWindow::removeCacheEvent(RemoveCacheEvent* event)
{
    // validate id or unknown ItemSection
    if (event->m_ItemID.isEmpty() || 
        event->m_CurrItemSection == ItemSections::Unknown)
        return;
    
    // cache id duplication in config.ini
    int counter = 0;
    std::wstring item_id = event->m_ItemID.toStdWString();
    if (item_id == m_Config.get(L"wallpaper", L"default_wallpaper_id"))
        counter++;
    if (item_id == m_Config.get(L"wallpaper", L"landscape_wallpaper_id"))
        counter++;
    if (item_id == m_Config.get(L"wallpaper", L"portrait_wallpaper_id"))
        counter++;
    
    // remove cache id in config.ini
    // Unknown and OthersWallpaper would not be process
    bool is_OthersWallpaper = false;
    switch (event->m_CurrItemSection)
    {
    case ItemSections::DefaultWallpaper:
        m_Config.set(L"wallpaper", L"default_wallpaper_id", L"");
        break;
    case ItemSections::LandscapeWallpaper:
        m_Config.set(L"wallpaper", L"landscape_wallpaper_id", L"");
        break;
    case ItemSections::PortraitWallpaper:
        m_Config.set(L"wallpaper", L"portrait_wallpaper_id", L"");
        break;
    case ItemSections::OthersWallpaper:
        is_OthersWallpaper = true;
        break;
    }
    
    // We cannot delete cache file while CacheBrowserDlg
    // is using them. So we need to store WallpaperIDs that
    // need to be delete and delete them after CacheBrowserDlg
    // has finish exec(). See MainWindow::onBrowseCacheBntPressed()
    // for reference
    // 
    // update delete buffer only after
    // there are no duplication in 3 recorded
    // wallpaper_id in config
    ReloadWallpapersEvent *output_event = nullptr;
    // no duplication or is OthersWallpaper, remove cached wallpaper file
    if (counter == 1 || is_OthersWallpaper) {
        m_DeleteBuff.push_back(event->m_ItemID);
        output_event = new ReloadWallpapersEvent();
    }
    else if (counter > 1) { // manually reset ctrls if no it will be remove 
        WallpaperTab_resetCtrls();
        output_event = new ReloadWallpapersEvent(
            ReloadWallpaperTasks::RemoveFromCache_NoFileDeletion,
            reinterpret_cast<void*>(new QString(event->m_ItemID))
        );
    }
    
    // notify CacheBrowser to update
    QApplication::instance()->postEvent(
        event->p_EventSource,
        output_event
    );
    qApp->processEvents();
}
void MainWindow::editCacheEvent(EditCacheEvent* event)
{
    // clean possible output wallpaper before enter function
    CleanCache(L"cropped_img.BMP");
    
    // declare buffers
    QString exe_path = QString::fromWCharArray(GlobTools::getCurrExePathW().c_str());
    QString file_path = exe_path + "Wallpapers\\" + event->m_ItemID;
    
    // launch ImageEditorDlg
    p_ImageEditorDlg = new ImageEditorDlg("ImageEditor", this);
    p_ImageEditorDlg->openImage(file_path);
    p_ImageEditorDlg->setClosePath(exe_path + "cropped_img.BMP");
    int result = p_ImageEditorDlg->exec();
    
    // after dlg closed
    if (result == QDialog::Rejected) {
        // add output file to Others
        add_image_as_wallpaper(exe_path + "cropped_img.BMP");
        // rm output file
        CleanCache(L"cropped_img.BMP");
        // modify newly added img profile
        GlobTools::utf8_wifstream WallpaperList_in(
            (exe_path + "Wallpapers\\WallpaperList").toStdWString()
        );
        // continue if able to read from WallpaperList
        if (!WallpaperList_in.fail()) {
            std::vector<std::wstring> temp_container;
            std::wstring buff;
            std::wstring buff2;
            // store all data from ./Wallpapers/WallpaperList to temp_container
            while (std::getline(WallpaperList_in, buff)) {
                temp_container.push_back(buff);
                buff.clear();
            }
            WallpaperList_in.close();
            // modify last line of temp_container
            // make the 1st section (src_path) to be new modified file's id
            buff = *(temp_container.end()-1);
            size_t pos1 = buff.find(L'\"') + 1;
            size_t pos2 = buff.find(L'\"', pos1) - 1;
            // get file's new id as its new filename
            size_t pos3 = pos2 + 4;
            size_t pos4 = buff.find(L'\"', pos3);
            buff2.assign(buff.begin()+pos3, buff.begin()+pos4);
            buff2 = (exe_path + "Wallpapers\\").toStdWString() + buff2;
            buff.replace(pos1, pos2, buff2);
            *(temp_container.end()-1) = buff;
            // write modified WallpaperList back to file
            GlobTools::utf8_wofstream WallpaperList_out(
                (exe_path + "Wallpapers\\WallpaperList").toStdWString()
            );
            for (auto it : temp_container)
                WallpaperList_out << it << "\n";
            WallpaperList_out.close();
        }
    }
    
    // notify CacheBrowser to update
    QApplication::instance()->postEvent(
        event->p_EventSource,
        new ReloadWallpapersEvent()
    );
    qApp->processEvents();
}
void MainWindow::cacheInfoEvent(CacheInfoEvent* event)
{
    // find cache file info
    QObject parent;
    QString program = QString::fromWCharArray(m_Config.get(L"program", L"core_program").c_str());
    QStringList arguments;
    arguments
            << "--find"
            << event->m_ItemID
    ;
    QProcess myProcess(&parent);
    myProcess.start(program, arguments);
    // wait for core to finish
    while (!myProcess.waitForFinished())
        Sleep(500);
    myProcess.close();
    
    // read from core_cache
    Cache_ReaderW cache(L"core_cache");
    
    if (cache.isCacheExist()) {
        // output file info to MsgBox
        std::wstring buff;
        for (auto it : *cache.getData())
            buff += it + L'\n';
        MessageBoxW(0, buff.c_str(), L"File Information", 0);
    } else {
        MessageBoxW(0, L"Cannot Read File Information", L"File Information", 0);
    }
    
    // notify CacheBrowser to update
    QApplication::instance()->postEvent(
        event->p_EventSource,
        new ReloadWallpapersEvent()
    );
    qApp->processEvents();
}



// private:
void MainWindow::init_WallpaperTab()
{
    WallpaperTab_resetCtrls();
    WallpaperTab_makeConnections();
}
void MainWindow::WallpaperTab_resetCtrls()
{
    // keep m_ControlChanged not be modify by setText()
    bool stat_buff = m_ControlChanged;
    
    // setup "Wallpaper" tab
    mainwindowTab->setTabText(0, "Wallpaper");
    // clear all TextEdit
    defaultTextEdit->setText("");
    landscapeTextEdit->setText("");
    portraitTextEdit->setText("");
    // set TextEdit text
    is_all_wallpaper_set();
    
    m_ControlChanged = stat_buff;
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
QString MainWindow::add_default_wallpaper()
{
    QString temp_id;
    if (m_DefaultWallpaper.isEmpty())
        return "";
    else if (is_cached_file(m_DefaultWallpaper, temp_id)) {
        m_Config.set(L"wallpaper", L"default_wallpaper_id", temp_id.toStdWString());
        int ind = m_DeleteBuff.indexOf(temp_id);
        if (ind != -1)
            m_DeleteBuff.remove(ind);
        return temp_id;
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
    if (cache.isCacheExist() && cache.getData()->at(0) == L"1") {
        m_Config.set(L"wallpaper", L"default_wallpaper_id", cache.getData()->at(1));
    } else {
        MessageBoxW(0, L"No core_cache for Default", L"Info", 0);
    }
    
    // update control status 
    m_ControlChanged = true;
    
    return QString::fromWCharArray(cache.getData()->at(1).c_str());
}
void MainWindow::select_default_wallpaper()
{
    m_DefaultWallpaper = select_image("Select Default Wallpaper");
    if (!m_DefaultWallpaper.isEmpty())
        defaultTextEdit->setText(m_DefaultWallpaper);
    // update control status 
    m_ControlChanged = true;
}
QString MainWindow::add_landscape_wallpaper()
{
    QString temp_id;
    if (m_LandscapeWallpaper.isEmpty())
        return "";
    else if (is_cached_file(m_LandscapeWallpaper, temp_id)) {
        m_Config.set(L"wallpaper", L"landscape_wallpaper_id", temp_id.toStdWString());
        int ind = m_DeleteBuff.indexOf(temp_id);
        if (ind != -1)
            m_DeleteBuff.remove(ind);
        return temp_id;
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
    if (cache.isCacheExist() && cache.getData()->at(0) == L"1") {
        m_Config.set(L"wallpaper", L"landscape_wallpaper_id", cache.getData()->at(1));
    } else {
        MessageBoxW(0, L"No core_cache for Landscape", L"Info", 0);
    }    
    
    // update control status 
    m_ControlChanged = true;
    
    return QString::fromWCharArray(cache.getData()->at(1).c_str());
}
void MainWindow::select_landscape_wallpaper()
{
    m_LandscapeWallpaper = select_image("Select Landscape Wallpaper");
    if (!m_LandscapeWallpaper.isEmpty())
        landscapeTextEdit->setText(m_LandscapeWallpaper);
    // update control status 
    m_ControlChanged = true;
}
QString MainWindow::add_portrait_wallpaper()
{
    QString temp_id;
    if (m_PortraitWallpaper.isEmpty())
        return "";
    else if (is_cached_file(m_PortraitWallpaper, temp_id)) {
        m_Config.set(L"wallpaper", L"portrait_wallpaper_id", temp_id.toStdWString());
        int ind = m_DeleteBuff.indexOf(temp_id);
        if (ind != -1)
            m_DeleteBuff.remove(ind);
        return temp_id;
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
    if (cache.isCacheExist() && cache.getData()->at(0) == L"1") {
        m_Config.set(L"wallpaper", L"portrait_wallpaper_id", cache.getData()->at(1));
    } else {
        MessageBoxW(0, L"No core_cache for Portrait", L"Info", 0);
    }
    
    // update control status 
    m_ControlChanged = true;
    
    return QString::fromWCharArray(cache.getData()->at(1).c_str());
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
QString MainWindow::add_image_as_wallpaper(const QString& file_path)
{
    QString temp_id;
    if (file_path.isEmpty())
        return "";
    else if (is_cached_file(file_path, temp_id)) {
        int ind = m_DeleteBuff.indexOf(temp_id);
        if (ind != -1)
            m_DeleteBuff.remove(ind);
        return temp_id;
    }
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
    // clear core_cache & return
    Cache_ReaderW cache(L"core_cache");
    if (cache.isCacheExist())
        return QString::fromWCharArray(cache.getData()->at(1).c_str());
    else return QString();
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
    // get wallpaper ID from core_cache
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
