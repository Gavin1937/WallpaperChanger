#pragma once

// Qt Libs
#include <QApplication>
#include <QDialog>
#include <QDir>
#include <QEvent>
#include <QIcon>
#include <QImageReader>
#include <QListView>
#include <QPixmap>
#include <QResizeEvent>
#include <QSize>
#include <QStandardItemModel>
#include <QString>
#include <QVector>

// ui
#include "./ui/ui_cachebrowserdlg.h"

// C++ STL
#include <string>

// Windows API
#include <Windows.h>

// utilities
#include "../utilities/GlobTools.h"
#include "../utilities/ConfigManager.h"


// class declaration
class CacheBrowserDlg;
class ListView;


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
    
private: // helper functions
    void load_singleWallpaper(ListView *target_listView, const QString& WallpapersDir, const QString& wallpaperId);
    void load_multiWallpaper(QStandardItemModel* model, const QString& WallpapersDir, const QString& wallpaperId);
    
private:
    // ListView widgets
    ListView *p_DefaultListView;
    ListView *p_LandscapeListView;
    ListView *p_PortraitListView;
    ListView *p_OthersListView;
    QImageReader m_ImageReader;
    ConfigManager* p_ParentConfig;
    // wallpaper id for default, landscape, & portrait
    QVector<std::string> m_SavedWallpapers;
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
    
    // overloading setModel(), add automatically setFlag() to it
    virtual void setModel(QAbstractItemModel *model);
    
protected:
    // handle selection change event
    virtual void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
    
private:
    ListView* m_Friend1;
    ListView* m_Friend2;
    ListView* m_Friend3;
    bool m_IsProgrammatically;
};
