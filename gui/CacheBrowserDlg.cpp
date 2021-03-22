
// CacheBrowserDlg class implementation

#include "CacheBrowserDlg.h"


// ====================== CacheBrowserDlg ======================

CacheBrowserDlg::CacheBrowserDlg(ConfigManager* parent_config, QWidget* parent)
    : QDialog(parent),
    p_ParentObject(reinterpret_cast<QObject*>(parent)),
    p_SubMenu_AddFromCache(nullptr),
    p_Act_AddToDefault(nullptr),
    p_Act_AddToLandscape(nullptr),
    p_Act_AddToPortrait(nullptr),
    m_SelectedMenuItem(ItemSections::Unknown),
    p_DefaultListView(nullptr),
    p_LandscapeListView(nullptr),
    p_PortraitListView(nullptr),
    p_OthersListView(nullptr),
    m_ImageReader(QImageReader()),
    p_ParentConfig(parent_config),
    m_RemovedWallpapers(QVector<QString>())
{
    // set dlg to delete on close
    this->setAttribute(Qt::WA_DeleteOnClose);
    
    // setup ui & dlg size
    setupUi(this);
    this->setFixedSize(680, 700);
    
    // setup QImageReader
    m_ImageReader.setDecideFormatFromContent(true);
    // setup all ListView
    setup_ListView();
    // load cached wallpapers
    loadWallpapers();
    
    // setup Menu for Bnt_AddFromCache
    setup_Menu4AddFromCache();
    
    // set function button icon
    Bnt_AddFromComputer->setIcon(QIcon(":res/green_plus.png"));
    Bnt_AddFromCache->setIcon(QIcon(":res/orange_plus.png"));
    Bnt_RemoveCache->setIcon(QIcon(":res/red_minus.png"));
    Bnt_EditCache->setIcon(QIcon(":res/lightblue_edit.png"));
    Bnt_CacheInfo->setIcon(QIcon(":res/blue_question.png"));
    
    // make connections
    connect(Bnt_AddFromComputer, &QAbstractButton::released, this, &CacheBrowserDlg::onAddFromComputerPressed);
    connect(Bnt_RemoveCache, &QAbstractButton::released, this, &CacheBrowserDlg::onRemoveCachePressed);
    connect(Bnt_EditCache, &QAbstractButton::released, this, &CacheBrowserDlg::onEditCachePressed);
    connect(Bnt_CacheInfo, &QAbstractButton::released, this, &CacheBrowserDlg::onCacheInfoPressed);
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
        if (m_RemovedWallpapers.indexOf(
                QString::fromWCharArray(id_buff.c_str())
            ) == -1) { 
            load_singleWallpaper(
                    p_DefaultListView, 
                    WallpapersDir.absolutePath(), 
                    QString::fromWCharArray(id_buff.c_str()));
        }
        
        // load Landscape Wallpaper
        id_buff = p_ParentConfig->get(L"wallpaper", L"landscape_wallpaper_id");
        if (m_RemovedWallpapers.indexOf(
                QString::fromWCharArray(id_buff.c_str())
            ) == -1) { // cannot find Wallpaper in m_RemovedWallpapers 
            load_singleWallpaper(
                    p_LandscapeListView,
                    WallpapersDir.absolutePath(), 
                    QString::fromWCharArray(id_buff.c_str()));
        }
        
        // load Portrait Wallpaper
        id_buff = p_ParentConfig->get(L"wallpaper", L"portrait_wallpaper_id");
        if (m_RemovedWallpapers.indexOf(
                QString::fromWCharArray(id_buff.c_str())
            ) == -1) { // cannot find Wallpaper in m_RemovedWallpapers
                    load_singleWallpaper(
                    p_PortraitListView, 
                    WallpapersDir.absolutePath(), 
                    QString::fromWCharArray(id_buff.c_str()));
        }
        
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
            // current Wallpaper is not Default, Landscape, or Portrait
            if (m_SavedWallpapers.indexOf(buff) == -1 && 
                // current Wallpaper cannot be found in m_RemovedWallpapers
                m_RemovedWallpapers.indexOf(QString(buff.c_str())) == -1) 
            { 
                load_multiWallpaper(
                    others_model,
                    WallpapersDir.absolutePath(),
                    QString(buff.c_str()));
            } // ignore current line if current id is Default, Landscape, or Portrait
        }
    } else { // do not have /Wallpapers/ dir
        // load "missing_wallpaper.png" to all
        QStandardItemModel *missing_model = new QStandardItemModel(this);
        missing_model->appendRow(
            new QStandardItem(
                QIcon(":res/missing_wallpaper.png"),
                ""
        ));
        p_DefaultListView->setModel(missing_model);
        p_LandscapeListView->setModel(missing_model);
        p_PortraitListView->setModel(missing_model);
    }
}
// reload cached wallpapers
void CacheBrowserDlg::reloadWallpapers()
{
    this->clear_loadedWallpaper();
    this->loadWallpapers();
}


// public slots:

// button handle slots
void CacheBrowserDlg::onAddFromComputerPressed()
{
    QApplication::instance()->postEvent(
        p_ParentObject,
        new AddFileFromComputerEvent(this, getCurrItemSections())
    );
    qApp->processEvents();
}
// Handle QMenu Selection in p_SubMenu_AddFromCache
void CacheBrowserDlg::onMenuSelectDefault()
{
    m_SelectedMenuItem = ItemSections::DefaultWallpaper;
	QApplication::instance()->postEvent(
		p_ParentObject,
		new AddFileFromCacheEvent(this, 
            getCurrItemSections(), 
            getCurrSelectedItemID(), 
            m_SelectedMenuItem)
	);
	qApp->processEvents();
}
void CacheBrowserDlg::onMenuSelectLandscape()
{
    m_SelectedMenuItem = ItemSections::LandscapeWallpaper;
	QApplication::instance()->postEvent(
		p_ParentObject,
		new AddFileFromCacheEvent(this, 
            getCurrItemSections(), 
            getCurrSelectedItemID(), 
            m_SelectedMenuItem)
	);
	qApp->processEvents();
}
void CacheBrowserDlg::onMenuSelectPortrait()
{
    m_SelectedMenuItem = ItemSections::PortraitWallpaper;
	QApplication::instance()->postEvent(
		p_ParentObject,
		new AddFileFromCacheEvent(this, 
            getCurrItemSections(), 
            getCurrSelectedItemID(), 
            m_SelectedMenuItem)
	);
	qApp->processEvents();
}
// End Handle QMenu Selection in p_SubMenu_AddFromCache
void CacheBrowserDlg::onRemoveCachePressed()
{
    // record item id
    QString curr_item_id = getCurrSelectedItemID();
    ListView *curr_listview = getCurrSelectedListView();
    m_RemovedWallpapers.push_back(curr_item_id);
    
    // unload selected item
    curr_listview->unloadSelectedIcon();
    
    // update dlg
    reloadWallpapers();
    this->repaint();
    this->update();
	qApp->processEvents();
    
    // ask mainwindow to delete cache file after exit dlg 
    QApplication::instance()->postEvent(
        p_ParentObject,
        new RemoveCacheEvent(this, curr_item_id)
    );
    qApp->processEvents();
}
void CacheBrowserDlg::onEditCachePressed()
{
    QString curr_item_id = getCurrSelectedItemID();
    QApplication::instance()->postEvent(
        p_ParentObject,
        new EditCacheEvent(this, curr_item_id)
    );
    qApp->processEvents();
}
void CacheBrowserDlg::onCacheInfoPressed()
{
    QString curr_item_id = getCurrSelectedItemID();
    QApplication::instance()->postEvent(
        p_ParentObject,
        new CacheInfoEvent(this, curr_item_id)
    );
    qApp->processEvents();
}


// protected: // event handling functions

// custom event handler
void CacheBrowserDlg::customEvent(QEvent* event)
{
    if (ReloadWallpapersEvent* RW_event = dynamic_cast<ReloadWallpapersEvent*>(event))
        reloadWallpapersEvent(RW_event);
}

void CacheBrowserDlg::reloadWallpapersEvent(ReloadWallpapersEvent* event)
{
    // remove ID in CacheBrowserDlg::m_RemovedWallpapers buffer 
    // if we add the same wallpaper back
    if (event->p_Data != nullptr) {
        QString temp = *(reinterpret_cast<QString*>(event->p_Data));
        if (event->m_TaskName == "AddFromComputer") {
            int ind = m_RemovedWallpapers.indexOf(temp);
            if (ind != -1)
                m_RemovedWallpapers.remove(ind);
        }
    }
    reloadWallpapers();
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
                QIcon(QPixmap::fromImage(m_ImageReader.read().scaled(
                    200, 200,
                    Qt::KeepAspectRatio,
                    Qt::SmoothTransformation))
                ),
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
    target_listView->setHasItemFlag(true);
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
        m_ImageReader.setFileName(currWallpaperDir);
        model->appendRow(
            new QStandardItem(
                QIcon(QPixmap::fromImage(m_ImageReader.read().scaled(
                    200, 200,
                    Qt::KeepAspectRatio,
                    Qt::SmoothTransformation))
                ),
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
    p_OthersListView->setHasItemFlag(true);
}
void CacheBrowserDlg::clear_loadedWallpaper()
{
    if (p_DefaultListView->hasItem()) {
        reinterpret_cast<QStandardItemModel*>(p_DefaultListView->model())->clear();
        p_DefaultListView->setHasItemFlag(false);
    }
    if (p_LandscapeListView->hasItem()) {
        reinterpret_cast<QStandardItemModel*>(p_LandscapeListView->model())->clear();
        p_LandscapeListView->setHasItemFlag(false);
    }
    if (p_PortraitListView->hasItem()) {
        reinterpret_cast<QStandardItemModel*>(p_PortraitListView->model())->clear();
        p_PortraitListView->setHasItemFlag(false);
    }
    m_SavedWallpapers.clear();
    if (p_OthersListView->hasItem()) {
        reinterpret_cast<QStandardItemModel*>(p_OthersListView->model())->clear();
        p_OthersListView->setHasItemFlag(false);
    }
}

// get ListView Item Selection Item
const ItemSections CacheBrowserDlg::getCurrItemSections()
{
    // find current selected ListView
    if (p_DefaultListView->hasSelection())
        return ItemSections::DefaultWallpaper;
    else if (p_LandscapeListView->hasSelection())
        return ItemSections::LandscapeWallpaper;
    else if (p_PortraitListView->hasSelection())
        return ItemSections::PortraitWallpaper;
    else if (p_OthersListView->hasSelection())
        return ItemSections::OthersWallpaper;
    return ItemSections::Unknown;
}
QStandardItem* CacheBrowserDlg::getCurrSelectedItem()
{
    // find current selected ListView
    QStandardItemModel* loc_model = nullptr;
    QModelIndex loc_ind;
    QStandardItem* loc_item = nullptr;
    if (p_DefaultListView->hasSelection()) {
        loc_model = reinterpret_cast<QStandardItemModel*>(p_DefaultListView->model());
        if (loc_model == nullptr)
            return nullptr;
        loc_item = loc_model->item(0);
    }
    else if (p_LandscapeListView->hasSelection()) {
        loc_model = reinterpret_cast<QStandardItemModel*>(p_LandscapeListView->model());
        if (loc_model == nullptr)
            return nullptr;
		loc_item = loc_model->item(0);
    }
    else if (p_PortraitListView->hasSelection()) {
        loc_model = reinterpret_cast<QStandardItemModel*>(p_PortraitListView->model());
        if (loc_model == nullptr)
            return nullptr;
		loc_item = loc_model->item(0);
    }
    else if (p_OthersListView->hasSelection()) {
        loc_model = reinterpret_cast<QStandardItemModel*>(p_OthersListView->model());
        if (loc_model == nullptr)
            return nullptr;
        loc_ind = p_OthersListView->selectionModel()->currentIndex();
        loc_item = loc_model->itemFromIndex(loc_ind);
    }
    // output
    if (loc_item != nullptr)
        return loc_item;
    else
        return nullptr;
}
ListView* CacheBrowserDlg::getCurrSelectedListView()
{
    // find current selected ListView
    if (p_DefaultListView->hasSelection())
        return p_DefaultListView;
    else if (p_LandscapeListView->hasSelection())
        return p_LandscapeListView;
    else if (p_PortraitListView->hasSelection())
        return p_PortraitListView;
    else if (p_OthersListView->hasSelection())
        return p_OthersListView;
    return nullptr;
}
QString CacheBrowserDlg::getCurrSelectedItemID()
{
    QStandardItem* loc_item = getCurrSelectedItem();
    if (loc_item != nullptr)
        return loc_item->text();
    else return QString();
}
void CacheBrowserDlg::setup_Menu4AddFromCache()
{
    p_SubMenu_AddFromCache = new QMenu(Bnt_AddFromCache);
    // Adding Actions
    // Default
    p_Act_AddToDefault = new QAction("Default Wallpaper");
    connect(p_Act_AddToDefault, &QAction::triggered, this, &CacheBrowserDlg::onMenuSelectDefault);
    p_SubMenu_AddFromCache->addAction(p_Act_AddToDefault);
    // Landscape
    p_Act_AddToLandscape = new QAction("Landscape Wallpaper");
    connect(p_Act_AddToLandscape, &QAction::triggered, this, &CacheBrowserDlg::onMenuSelectLandscape);
    p_SubMenu_AddFromCache->addAction(p_Act_AddToLandscape);
    // Portrait
    p_Act_AddToPortrait = new QAction("Portrait Wallpaper");
    connect(p_Act_AddToPortrait, &QAction::triggered, this, &CacheBrowserDlg::onMenuSelectPortrait);
    p_SubMenu_AddFromCache->addAction(p_Act_AddToPortrait);
    // set Menu
    Bnt_AddFromCache->setPopupMode(QToolButton::InstantPopup);
    Bnt_AddFromCache->setMenu(p_SubMenu_AddFromCache);
}


// ====================== CacheBrowserDlg End ======================





// ====================== ListView End ======================

// public:

// constructor
ListView::ListView(QWidget* parent)
    : QListView(parent), m_Friend1(nullptr), m_Friend2(nullptr), m_Friend3(nullptr),
    m_IsProgrammatically(false), m_HasItem(false)
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

const bool ListView::hasSelection()
{
    return !(this->QListView::selectedIndexes().empty());
}
void ListView::setHasItemFlag(const bool& flag)
{
    m_HasItem = flag;
}
const bool ListView::hasItem()
{
    return m_HasItem;
}

// overloading setModel(), add automatically setFlag() to it
void ListView::setModel(QAbstractItemModel *model)
{
    this->setFlag(true);
    this->QListView::setModel(model);
}

// unload selected icon in ListView
void ListView::unloadSelectedIcon()
{
    QModelIndex curr_ind = this->selectionModel()->currentIndex();
    QStandardItemModel* curr_model =
        reinterpret_cast<QStandardItemModel*>(this->model());
    curr_model->itemFromIndex(curr_ind)->setIcon(QIcon(""));
    curr_model->itemFromIndex(curr_ind)->setText("");
}


// ====================== ListView End ======================





// ====================== Custom Events ======================

ReloadWallpapersEvent::ReloadWallpapersEvent(QString taskName, void* return_data)
    : QEvent(RELOAD_WALLPAPERS_EVENT_TYPE),
    m_TaskName(taskName),
    p_Data(return_data)
{}
ReloadWallpapersEvent::~ReloadWallpapersEvent()
{
    if (p_Data != nullptr)
        delete p_Data;
}
AddFileFromComputerEvent::AddFileFromComputerEvent(
    CacheBrowserDlg* eventSource,
    const ItemSections& itemSection)
    : QEvent(ADD_FILE_FROM_COMPUTER_EVENT_TYPE),
    p_EventSource(reinterpret_cast<QObject*>(eventSource)),
    m_ItemSection(itemSection)
{}
AddFileFromCacheEvent::AddFileFromCacheEvent(
        CacheBrowserDlg* eventSource,
        const ItemSections& curr_itemSection,
        const QString& currItemID,
        const ItemSections& to_itemSection)
    : QEvent(ADD_FILE_FROM_CACHE_EVENT_TYPE),
    p_EventSource(reinterpret_cast<QObject*>(eventSource)),
    m_CurrItemSection(curr_itemSection),
    m_CurrItemID(currItemID),
    m_ToItemSection(to_itemSection)
{}
RemoveCacheEvent::RemoveCacheEvent(
    CacheBrowserDlg* eventSource, 
    const QString& itemID)
    : QEvent(REMOVE_CACHE_EVENT_TYPE),
    p_EventSource(reinterpret_cast<QObject*>(eventSource)),
    m_ItemID(itemID)
{}
EditCacheEvent::EditCacheEvent(
    CacheBrowserDlg* eventSource, 
    const QString& itemID)
    : QEvent(EDIT_CACHE_EVENT_TYPE),
    p_EventSource(reinterpret_cast<QObject*>(eventSource)),
    m_ItemID(itemID)
{}
CacheInfoEvent::CacheInfoEvent(
    CacheBrowserDlg* eventSource, 
    const QString& itemID)
    : QEvent(CACHE_INFO_EVENT_TYPE),
    p_EventSource(reinterpret_cast<QObject*>(eventSource)),
    m_ItemID(itemID)
{}


// ====================== Custom Events End ======================