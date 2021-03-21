#pragma once

// Qt Libs
#include <QAction>
#include <QApplication>
#include <QCloseEvent>
#include <QDialog>
#include <QDir>
#include <QEvent>
#include <QEvent>
#include <QIcon>
#include <QImageReader>
#include <QListView>
#include <QMenu>
#include <QModelIndex>
#include <QPixmap>
#include <QResizeEvent>
#include <QSize>
#include <QStandardItemModel>
#include <QString>
#include <QToolButton>
#include <QVector>

// ui
#include "./ui/ui_cachebrowserdlg.h"

// C++ STL
#include <string>

// Windows API
#include <Windows.h>

// utilities
#include "Qt_CustomEventID.h"
#include "../utilities/GlobTools.h"
#include "../utilities/ConfigManager.h"


// class declaration
class CacheBrowserDlg;
class ListView;
// custom events
// receiving
class ReloadWallpapersEvent;
// sending
class AddFileFromComputerEvent;
class AddFileFromCacheEvent;
class RemoveCacheEvent;
class EditCacheEvent;
class CacheInfoEvent;

// struct & enum declaration
enum class ItemSections {
    Unknown = 0,
    DefaultWallpaper,
    LandscapeWallpaper,
    PortraitWallpaper,
    OthersWallpaper
};


// Dialog for Browsing Cached Wallpapers under ${PROGRAM_ROOT}/Wallpapers/
class CacheBrowserDlg : public QDialog, public Ui_CacheBrowser
{
    Q_OBJECT


public:
    CacheBrowserDlg(ConfigManager* parent_config, QWidget* parent = nullptr);
    ~CacheBrowserDlg();
    
    // setup all 3 ListView
    void setup_ListView();
    
    // loading cached wallpapers
    void loadWallpapers();
    // reload cached wallpapers
    void reloadWallpapers();
    
public slots:
    // button handle slots
    void onAddFromComputerPressed();
    // Handle QMenu Selection in p_SubMenu_AddFromCache
    void onMenuSelectDefault();
    void onMenuSelectLandscape();
    void onMenuSelectPortrait();
    // End Handle QMenu Selection in p_SubMenu_AddFromCache
    void onRemoveCachePressed();
    void onEditCachePressed();
    void onCacheInfoPressed();
    
protected: // event handling functions
    // custom event handler
    void customEvent(QEvent* event);
    
    void reloadWallpapersEvent(ReloadWallpapersEvent* event);
    
private: // helper functions
    void load_singleWallpaper(ListView *target_listView, const QString& WallpapersDir, const QString& wallpaperId);
    void load_multiWallpaper(QStandardItemModel* model, const QString& WallpapersDir, const QString& wallpaperId);
    void clear_loadedWallpaper();
    
    // get ListView Item Selection Item
    const ItemSections getCurrItemSections();
    QStandardItem* getCurrSelectedItem();
    ListView* getCurrSelectedListView();
    void setup_Menu4AddFromCache();
    
private:
    // parent QObject
    QObject *p_ParentObject;
    // Menu for Bnt_AddFromCache
    QMenu *p_SubMenu_AddFromCache;
    // actions for p_SubMenu_AddFromCache
    QAction *p_Act_AddToDefault;
    QAction *p_Act_AddToLandscape;
    QAction *p_Act_AddToPortrait;
    // Store Current Selection from p_SubMenu_AddFromCache
    ItemSections m_SelectedMenuItem;
    // ListView widgets
    ListView *p_DefaultListView;
    ListView *p_LandscapeListView;
    ListView *p_PortraitListView;
    ListView *p_OthersListView;
    QImageReader m_ImageReader;
    ConfigManager* p_ParentConfig;
    // wallpaper id for default, landscape, & portrait
    QVector<std::string> m_SavedWallpapers;
    // Temporary Buffer for removed Wallpaper ID
    QVector<QString> m_RemovedWallpapers;
};



// custom QListView for single selection across multiple QListView
class ListView : public QListView
{
public:
    // constructor
    ListView(QWidget* parent = nullptr);
    
    // set friend ListView
    void setFriends(ListView* friend1, ListView* friend2, ListView* friend3);
    
    // clear selection on current ListView
    void clearSelection();
    
    // set m_IsProgrammatically flag
    void setFlag(const bool& flag);
    
    const bool hasSelection(); 
    void setHasItemFlag(const bool& flag);
    const bool hasItem();
    
    // overloading setModel(), add automatically setFlag() to it
    virtual void setModel(QAbstractItemModel *model);
    
    // unload selected icon in ListView
    void unloadSelectedIcon();
    
protected:
    // handle selection change event
    virtual void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
    
private:
    ListView* m_Friend1;
    ListView* m_Friend2;
    ListView* m_Friend3;
    bool m_IsProgrammatically;
    bool m_HasItem;
};


// Custom Events
// receiving
class ReloadWallpapersEvent : public QEvent
{
public:
    ReloadWallpapersEvent(
        QString taskName = "none", 
        void* return_data = nullptr
    );
    ~ReloadWallpapersEvent();
    QString m_TaskName;
    void *p_Data;
};
// sending
class AddFileFromComputerEvent : public QEvent
{
public:
    AddFileFromComputerEvent(CacheBrowserDlg* eventSource, const ItemSections& itemSection);
    QObject* p_EventSource;
    const ItemSections m_ItemSection;
};
class AddFileFromCacheEvent : public QEvent
{
public:
    AddFileFromCacheEvent(
        CacheBrowserDlg* eventSource,
        const ItemSections& curr_itemSection,
        const QString& currItemID,
        const ItemSections& to_itemSection
    );
    QObject* p_EventSource;
    const ItemSections m_CurrItemSection;
    QString m_CurrItemID;
    const ItemSections m_ToItemSection;
};
class RemoveCacheEvent : public QEvent
{
public:
    RemoveCacheEvent(CacheBrowserDlg* eventSource, const QString& itemID);
    QObject* p_EventSource;
    const QString m_ItemID;
};
class EditCacheEvent : public QEvent
{
public:
    EditCacheEvent(CacheBrowserDlg* eventSource, const QString& itemID);
    QObject* p_EventSource;
    const QString m_ItemID;
};
class CacheInfoEvent : public QEvent
{
public:
    CacheInfoEvent(CacheBrowserDlg* eventSource, const QString& itemID);
    QObject* p_EventSource;
    const QString m_ItemID;
};
