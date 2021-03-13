#pragma once

// Qt Libs
#include <QApplication>
#include <QDialog>
#include <QEvent>
#include <QResizeEvent>
#include <QIcon>

// ui
#include "./ui/ui_cachebrowserdlg.h"



// Dialog for Browsing Cached Wallpapers under ${PROGRAM_ROOT}/Wallpapers/
class CacheBrowserDlg : public QDialog, public Ui_CacheBrowser
{
    Q_OBJECT


public:
    CacheBrowserDlg(QWidget* parent = nullptr);
    
    // virtual int	exec();
    
protected:
    // virtual void resizeEvent(QResizeEvent* event);
    
private:
    // void resetDlgSize();
private:
    bool m_IsProgrammatically;
};