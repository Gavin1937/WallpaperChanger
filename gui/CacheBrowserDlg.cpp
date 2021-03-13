
// CacheBrowserDlg class implementation

#include "CacheBrowserDlg.h"


// ====================== CacheBrowserDlg ======================

CacheBrowserDlg::CacheBrowserDlg(QWidget* parent)
    : QDialog(parent),
    m_IsProgrammatically(true)
{
    // this->setWindowTitle("WallpaperChanger");
    setupUi(this);
    
    this->setFixedSize(680, 700);
    
    // set function button icon
    Bnt_AddFromComputer->setIcon(QIcon(":res/green_plus.png"));
    Bnt_AddFromCache->setIcon(QIcon(":res/orange_plus.png"));
    Bnt_RemoveCache->setIcon(QIcon(":res/red_minus.png"));
    Bnt_CacheInfo->setIcon(QIcon(":res/blue_question.png"));
    
    // resetDlgSize();
}

// int	CacheBrowserDlg::exec()
// {
//     resetDlgSize();
//     m_IsProgrammatically = false;
//     return this->QDialog::exec();
// }


// // private
// void CacheBrowserDlg::resetDlgSize()
// {
//     m_IsProgrammatically = true;
    
//     this->resize(640, 700);
    
//     // default
//     Label_Default->setGeometry(20, 20, 161, 21);
//     ListView_Default->setGeometry(10, 50, 200, 200);
//     ListView_Default->resize(200, 200);
//     // landscape
//     Label_Landscape->setGeometry(230, 20, 181, 21);
//     ListView_Landscape->setGeometry(220, 50, 200, 200);
//     ListView_Landscape->resize(200, 200);
//     // portrait
//     Label_Portrait->setGeometry(440, 20, 161, 21);
//     ListView_Portrait->setGeometry(430, 50, 200, 200);
//     ListView_Portrait->resize(200, 200);
//     // other
//     Label_Others->setGeometry(20, 260, 161, 21);
//     ListView_Others->setGeometry(10, 290, 620, 400);
//     ListView_Others->resize(620, 400);
    
//     // this->setFixedSize(640, 700);
    
//     // m_IsProgrammatically = false;
// }

// // protected
// void CacheBrowserDlg::resizeEvent(QResizeEvent* event)
// {
//     if (m_IsProgrammatically)
//         return;
//     // basic size info
//     int old_W = event->oldSize().width();
//     int old_H = event->oldSize().height();
//     int new_W = event->size().width();
//     int new_H = event->size().height();
//     int delta_W = new_W - old_W;
//     int delta_H = new_H - old_H;
//     // reset geometry of controls
//     // fixed values
//     int new_DefListView_W = ListView_Default->size().width()+(delta_W/3);
//     int new_DefListView_H = ListView_Default->size().height()+(delta_H/2);
//     // default
//     Label_Default->setGeometry(20, 20, 161, 21);
//     ListView_Default->setGeometry(10, 50, new_DefListView_W, new_DefListView_H);
//     // landscape
//     Label_Landscape->setGeometry(10+new_DefListView_W+20, 20, 181, 21);
//     ListView_Landscape->setGeometry(10+new_DefListView_W+10, 50, new_DefListView_W, new_DefListView_H);
//     // portrait
//     Label_Portrait->setGeometry(10+new_DefListView_W*2+20+10, 20, 161, 21);
//     ListView_Portrait->setGeometry(10+new_DefListView_W*2+20, 50, new_DefListView_W, new_DefListView_H);
//     // other
//     Label_Others->setGeometry(20, 50+new_DefListView_H+10, 161, 21);
//     ListView_Others->setGeometry(10, 50+new_DefListView_H+10+21+10, new_DefListView_W*3+20, (new_H-20)-(50+new_DefListView_H+10+21+10));
// }


// ====================== CacheBrowserDlg End ======================
