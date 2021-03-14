
// CacheBrowserDlg class implementation

#include "CacheBrowserDlg.h"


// ====================== CacheBrowserDlg ======================

CacheBrowserDlg::CacheBrowserDlg(ConfigManager* parent_config, QWidget* parent)
    : QDialog(parent),
    p_DefaultListView(nullptr),
    p_LandscapeListView(nullptr),
    p_PortraitListView(nullptr),
    p_OthersListView(nullptr),
    m_ImageReader(QImageReader()),
    p_ParentConfig(parent_config)
{
    // setup ui & dlg size
    setupUi(this);
    this->setFixedSize(680, 700);
    
    // setup QImageReader
    m_ImageReader.setDecideFormatFromContent(true);
    // setup all ListView
    setup_ListView();
    // load cached wallpapers
    loadWallpapers();
    
    // set function button icon
    Bnt_AddFromComputer->setIcon(QIcon(":res/green_plus.png"));
    Bnt_AddFromCache->setIcon(QIcon(":res/orange_plus.png"));
    Bnt_RemoveCache->setIcon(QIcon(":res/red_minus.png"));
    Bnt_EditCache->setIcon(QIcon(":res/lightblue_edit.png"));
    Bnt_CacheInfo->setIcon(QIcon(":res/blue_question.png"));
}
CacheBrowserDlg::~CacheBrowserDlg()
{
    p_ParentConfig = nullptr;
}

void CacheBrowserDlg::setup_ListView()
{
    // construct
    p_DefaultListView = new ListView(this);
    p_LandscapeListView = new ListView(this);
    p_PortraitListView = new ListView(this);
    p_OthersListView = new ListView(this);
    // setup friends
    p_DefaultListView->setFriends(p_LandscapeListView, p_PortraitListView, p_OthersListView);
    p_LandscapeListView->setFriends(p_DefaultListView, p_PortraitListView, p_OthersListView);
    p_PortraitListView->setFriends(p_DefaultListView, p_LandscapeListView, p_OthersListView);
    p_OthersListView->setFriends(p_DefaultListView, p_LandscapeListView, p_PortraitListView);
    // disable double click editing
    p_DefaultListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    p_LandscapeListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    p_PortraitListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    p_OthersListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    
    // adjust setting for individual ListView
    // Default
    p_DefaultListView->setGeometry(50, 50, 200, 200);
    p_DefaultListView->setViewMode(QListView::IconMode);
    p_DefaultListView->setMovement(QListView::Static);
    p_DefaultListView->setIconSize(QSize(190, 175));
    p_DefaultListView->setResizeMode(QListView::Adjust);
    p_DefaultListView->setAcceptDrops(false);
    p_DefaultListView->setItemAlignment(Qt::AlignCenter);
    // Landscape
    p_LandscapeListView->setGeometry(260, 50, 200, 200);
    p_LandscapeListView->setViewMode(QListView::IconMode);
    p_LandscapeListView->setMovement(QListView::Static);
    p_LandscapeListView->setIconSize(QSize(190, 175));
    p_LandscapeListView->setResizeMode(QListView::Adjust);
    p_LandscapeListView->setAcceptDrops(false);
    p_LandscapeListView->setItemAlignment(Qt::AlignCenter);
    // Portrait
    p_PortraitListView->setGeometry(470, 50, 200, 200);
    p_PortraitListView->setViewMode(QListView::IconMode);
    p_PortraitListView->setMovement(QListView::Static);
    p_PortraitListView->setIconSize(QSize(190, 175));
    p_PortraitListView->setResizeMode(QListView::Adjust);
    p_PortraitListView->setAcceptDrops(false);
    p_PortraitListView->setItemAlignment(Qt::AlignCenter);
    // Others
    p_OthersListView->setGeometry(50, 290, 620, 400);
    p_OthersListView->setViewMode(QListView::IconMode);
    p_OthersListView->setMovement(QListView::Static);
    p_OthersListView->setIconSize(QSize(190, 175));
    p_OthersListView->setResizeMode(QListView::Adjust);
    p_OthersListView->setAcceptDrops(false);
    p_OthersListView->setItemAlignment(Qt::AlignCenter);
}

// loading cached wallpapers
void CacheBrowserDlg::loadWallpapers()
{
    QDir WallpapersDir((GlobTools::getCurrExePathA()+"Wallpapers").c_str());
    bool hasWallpapersDir = WallpapersDir.exists();
    if (hasWallpapersDir) { // has /Wallpapers/
        // load Default Wallpaper
        std::wstring id_buff = p_ParentConfig->get(L"wallpaper", L"default_wallpaper_id");
        load_singleWallpaper(
                p_DefaultListView, 
                WallpapersDir.absolutePath(), 
                QString::fromWCharArray(id_buff.c_str()));
        
        // load Landscape Wallpaper
        id_buff = p_ParentConfig->get(L"wallpaper", L"landscape_wallpaper_id");
        load_singleWallpaper(
                p_LandscapeListView,
                WallpapersDir.absolutePath(), 
                QString::fromWCharArray(id_buff.c_str()));
        
        // load Portrait Wallpaper
        id_buff = p_ParentConfig->get(L"wallpaper", L"portrait_wallpaper_id");
        load_singleWallpaper(
                p_PortraitListView, 
                WallpapersDir.absolutePath(), 
                QString::fromWCharArray(id_buff.c_str()));
        
        // load Other Wallpapers
        QStandardItemModel *others_model = new QStandardItemModel(this);
        p_OthersListView->setModel(others_model);
        GlobTools::utf8_ifstream WallpaperLists(GlobTools::getCurrExePathA()+"Wallpapers\\WallpaperList");
        std::string buff;
        getline(WallpaperLists, buff); // skip 1st line
        while (getline(WallpaperLists, buff)) {
            // get id from current line
            size_t pos1 = 0;
            size_t pos2 = 0;
            for (int i = 0; i < 3; ++i) {
                pos1 = buff.find('\"', pos1);
                pos1++;
            }
            pos2 = buff.find('\"', pos1);
            buff.assign(buff.begin()+pos1, buff.begin()+pos2);
            // load wallpaper if current id is not Default, Landscape, or Portrait
            if (m_SavedWallpapers.indexOf(buff) == -1) { 
                load_multiWallpaper(
                    others_model,
                    WallpapersDir.absolutePath(),
                    QString(buff.c_str()));
            } // ignore current line if current id is Default, Landscape, or Portrait
        }
    }
}


// private: helper functions

void CacheBrowserDlg::load_singleWallpaper(
        ListView *target_listView,
        const QString& WallpapersDir,
        const QString& wallpaperId)
{
    QString currWallpaperDir = WallpapersDir + "/" + wallpaperId;
    if (!wallpaperId.isEmpty() && // has default wallpaper id
        GlobTools::is_filedir_existA(currWallpaperDir.toStdString())) // wallpaper exist
    { 
        m_SavedWallpapers.push_back(wallpaperId.toStdString());
        m_ImageReader.setFileName(currWallpaperDir);
        QStandardItemModel *model = new QStandardItemModel(this);
        model->appendRow(
            new QStandardItem(
                QIcon(QPixmap::fromImage(m_ImageReader.read())),
                wallpaperId
        ));
        target_listView->setModel(model);
    } else if (wallpaperId.isEmpty()) { // do not have wallpaper id, wallpaper exist
        QStandardItemModel *missing_model = new QStandardItemModel(this);
        missing_model->appendRow(
            new QStandardItem(
                QIcon(":res/missing_wallpaper.png"),
                ""
        ));
        target_listView->setModel(missing_model);
    } else { // wallpaper does not exist, have wallpaper id
        QStandardItemModel *missing_model = new QStandardItemModel(this);
        missing_model->appendRow(
            new QStandardItem(
                QIcon(":res/missing_wallpaper.png"),
                wallpaperId
        ));
        target_listView->setModel(missing_model);
    }
}
void CacheBrowserDlg::load_multiWallpaper(
        QStandardItemModel* model,
        const QString& WallpapersDir,
        const QString& wallpaperId)
{
    QString currWallpaperDir = WallpapersDir + "/" + wallpaperId;
    if (!wallpaperId.isEmpty() && // has default wallpaper id
        GlobTools::is_filedir_existA(currWallpaperDir.toStdString())) // wallpaper exist
    { 
        m_SavedWallpapers.push_back(wallpaperId.toStdString());
        m_ImageReader.setFileName(currWallpaperDir);
        model->appendRow(
            new QStandardItem(
                QIcon(QPixmap::fromImage(m_ImageReader.read())),
                wallpaperId
        ));
    } else if (wallpaperId.isEmpty()) { // do not have wallpaper id, wallpaper exist
        model->appendRow(
            new QStandardItem(
                QIcon(":res/missing_wallpaper.png"),
                "Unknown File"
        ));
    } else { // wallpaper does not exist, have wallpaper id
        model->appendRow(
            new QStandardItem(
                QIcon(":res/missing_wallpaper.png"),
                wallpaperId
        ));
    }
}


// ====================== CacheBrowserDlg End ======================





// ====================== ListView End ======================

// public:

// constructor
ListView::ListView(QWidget* parent)
    : QListView(parent), m_Friend1(nullptr), m_Friend2(nullptr), m_Friend3(nullptr),
    m_IsProgrammatically(false)
{}

// set friend ListView
void ListView::setFriends(ListView* friend1, ListView* friend2, ListView* friend3)
{
    m_Friend1 = friend1;
    m_Friend2 = friend2;
    m_Friend3 = friend3;
}

// clear selection on current ListView
void ListView::clearSelection()
{
    m_IsProgrammatically = true;
    if (this->selectionModel() != nullptr)
        this->selectionModel()->clearSelection();
    m_IsProgrammatically = false;
}

// set m_IsProgrammatically flag
void ListView::setFlag(const bool& flag)
{
    m_IsProgrammatically = flag;
}


// protected:

// handle selection change event
void ListView::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    if (!m_IsProgrammatically) {
        m_Friend1->clearSelection();
        m_Friend2->clearSelection();
        m_Friend3->clearSelection();
    }
    m_IsProgrammatically = false;
    this->QListView::selectionChanged(selected, deselected);
}

// overloading setModel(), add automatically setFlag() to it
void ListView::setModel(QAbstractItemModel *model)
{
    this->setFlag(true);
    this->QListView::setModel(model);
}


// ====================== ListView End ======================
