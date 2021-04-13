#pragma once

#include "ImageEditor.h"

#include <QDialog>
#include <QGridLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QCloseEvent>
#include <QPaintEvent>
#include <QContextMenuEvent>
#include <QKeyEvent>
#include <QMenu>


// class declaration
class ImageEditorDlg;
class My_PushButton;
class My_CheckBox;


class ImageEditorDlg : public QDialog
{
    Q_OBJECT

public:
    // default constructor
    ImageEditorDlg(
        const QString& dlg_title = "ImageEditor",
        QWidget* parent = nullptr
    );
    
    void openImage(const QString& path);
    
    void setClosePath(const QString& path);
    
protected: // event handling
    virtual void closeEvent(QCloseEvent *event);
    virtual void paintEvent(QPaintEvent* event);
    virtual void contextMenuEvent(QContextMenuEvent *e);
    virtual void keyPressEvent(QKeyEvent *event);

private: // helper function
    void saveNew();
    void Toggled();
    
    void setup_InteractiveWidgets();
    void make_Connection();
    void setup_WidgetsLayout();
    
private:
    QString m_ImageOpenPath;
    QString m_ImageClosePath;
    QString m_OriginalTitle; // original dlg title
    QSize m_DlgSize;
    ImageEditor *ie;
    QMenu *Menu;
    // interactive widgets
    My_PushButton *RotateL90;
    My_PushButton *RotateR90;
    My_PushButton *RotateCroppingBox;
    My_PushButton *MirrorH;
    My_PushButton *MirrorV;
    My_PushButton *Undo;
    My_PushButton *Crop;
    My_PushButton *Reset;
    My_CheckBox *Ratio;
};



// custom widgets that ignore keyPressEvent

// QPushButton doesn't handle keyPressEvent()
class My_PushButton : public QPushButton 
{
public:
    // constructor
    My_PushButton(QWidget* parent);
protected: // event handler
    virtual void keyPressEvent(QKeyEvent *e);
};


// QCheckBox doesn't handle keyPressEvent()
class My_CheckBox : public QCheckBox
{
public:
    // constructor
    My_CheckBox(QWidget* parent);
protected: // event handler
    virtual void keyPressEvent(QKeyEvent *e);
};
