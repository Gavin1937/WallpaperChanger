
#include "ImageEditorDlg.h"

#include <QVBoxLayout>
#include <QImage>
#include <QKeySequence>


ImageEditorDlg::ImageEditorDlg(
    const QString& dlg_title,
    QWidget* parent
)
    : QDialog(parent),
    m_ImageOpenPath(QString()),
    m_ImageClosePath(QString()),
    m_OriginalTitle(dlg_title),
    m_DlgSize(QSize()),
    ie(new ImageEditor(this)),
    Menu(new QMenu(this)),
    RotateL90(new My_PushButton(this)),
    RotateR90(new My_PushButton(this)),
    RotateCroppingBox(new My_PushButton(this)),
    MirrorH(new My_PushButton(this)),
    MirrorV(new My_PushButton(this)),
    Undo(new My_PushButton(this)),
    Crop(new My_PushButton(this)),
    Reset(new My_PushButton(this)),
    Ratio(new My_CheckBox(this))
{
    // set dlg to delete on close
    this->setAttribute(Qt::WA_DeleteOnClose);
    
    m_DlgSize = get_screen_res();
    // scale dialog resolution to screen resolution's 60%
    m_DlgSize *= 0.6; 
    // set fixed dialog size
    this->setFixedSize(m_DlgSize); 
    
    // set default output path for image
    m_ImageClosePath = qApp->applicationDirPath();
    int ind = m_ImageClosePath.indexOf('/');
    while (ind != -1) {
        m_ImageClosePath[ind] = '\\';
        ind = m_ImageClosePath.indexOf('/');
    }
    // add default image name
    m_ImageClosePath += "\\cropped_img.BMP";
    
    // init widgets under dlg
    setup_InteractiveWidgets();
}


void ImageEditorDlg::openImage(const QString& path)
{
    m_ImageOpenPath = path;
    // set image to ie
    ie->setImage(m_ImageOpenPath);
}
void ImageEditorDlg::setClosePath(const QString& path)
{
    if (!path.isEmpty())
        m_ImageClosePath = path;
}


// protected: // event handling
void ImageEditorDlg::closeEvent(QCloseEvent *event)
{
    // save current image on close
    ie->saveAs(m_ImageClosePath);
    qApp->processEvents();
    
    // accept dlg close event
    event->accept();
}
void ImageEditorDlg::paintEvent(QPaintEvent* event)
{
    // display cropping rect info on dlg title
    QString capture = m_OriginalTitle;
    QRectF ori = ie->getImageRect().Original();
    capture += 
            " - TopLeft(" + QString::number(ori.topLeft().x()) + 
            ", " + QString::number(ori.topLeft().y()) +
            ") - BottomRight(" + QString::number(ori.bottomRight().x()) +
            ", " + QString::number(ori.bottomRight().y()) +
            ")";
    this->setWindowTitle(capture);
    
    // continue to other paintEvent
    this->QDialog::paintEvent(event);
}
void ImageEditorDlg::contextMenuEvent(QContextMenuEvent *e)
{
    // handling left mouse click event
    QAction *Action1 = new QAction("Rotate Left 90");
    QAction *Action2 = new QAction("Rotate Right 90");
    QAction *Action3 = new QAction("Mirror Horizontal");
    QAction *Action4 = new QAction("Mirror Vertical");
    QAction *Action5 = new QAction("Undo");
    QAction *Action6 = new QAction("Crop");
    QAction *Action7 = new QAction("Reset Image");
    QAction *Action8 = new QAction("Reset Cropping Rectangle");
    
    connect(Action1, &QAction::triggered, ie, &ImageEditor::rotateL90);
    connect(Action2, &QAction::triggered, ie, &ImageEditor::rotateR90);
    connect(Action3, &QAction::triggered, ie, &ImageEditor::mirrorHori);
    connect(Action4, &QAction::triggered, ie, &ImageEditor::mirrorVrti);
    connect(Action5, &QAction::triggered, ie, &ImageEditor::undo);
    connect(Action6, &QAction::triggered, ie, &ImageEditor::crop);
    connect(Action7, &QAction::triggered, ie, &ImageEditor::reset);
    connect(Action8, &QAction::triggered, ie, &ImageEditor::resetCroppingRect);
    
    Menu->addAction(Action1);
    Menu->addAction(Action2);
    Menu->addAction(Action3);
    Menu->addAction(Action4);
    Menu->addAction(Action5);
    Menu->addAction(Action6);
    Menu->addAction(Action7);
    Menu->addAction(Action8);
    
    Menu->exec(QCursor::pos());
    Menu->clear();
}
void ImageEditorDlg::keyPressEvent(QKeyEvent* event)
{
    // Ctrl + Z shortcut, representing undo
    if (event->matches(QKeySequence::Undo)) 
        ie->undo();
    // Moving Cropping Rect with Arrow keys
    else if (event->key() == Qt::Key_Up) { // Up Arrow
        ie->moveCroppingRect(QPoint(0, -1));
    }
    else if (event->key() == Qt::Key_Down) { // Down Arrow
        ie->moveCroppingRect(QPoint(0, 1));
    }
    else if (event->key() == Qt::Key_Left) { // Left Arrow
        ie->moveCroppingRect(QPoint(-1, 0));
    }
    else if (event->key() == Qt::Key_Right) { // Right Arrow
        ie->moveCroppingRect(QPoint(1, 0));
    }
}


void ImageEditorDlg::Toggled()
{
    ie->setFixWHRatio2Screen(Ratio->checkState() == Qt::Checked);
}


void ImageEditorDlg::setup_InteractiveWidgets()
{
    // set widget hover text
    RotateL90->setToolTip("Rotate Left 90");
    RotateL90->setIcon(QIcon(":res/ImageEditorDlg/rotate_l.png"));
    RotateR90->setToolTip("Rotate Right 90");
    RotateR90->setIcon(QIcon(":res/ImageEditorDlg/rotate_r.png"));
    RotateCroppingBox->setToolTip("Rotate Cropping Box");
    RotateCroppingBox->setIcon(QIcon(":res/ImageEditorDlg/rotate_cropping_rect.png"));
    MirrorH->setToolTip("Mirror Horizontal");
    MirrorH->setIcon(QIcon(":res/ImageEditorDlg/mirror_h.png"));
    MirrorV->setToolTip("Mirror Vertical");
    MirrorV->setIcon(QIcon(":res/ImageEditorDlg/mirror_v.png"));
    Undo->setToolTip("Undo");
    Undo->setIcon(QIcon(":res/ImageEditorDlg/undo.png"));
    Crop->setToolTip("Crop");
    Crop->setIcon(QIcon(":res/ImageEditorDlg/crop.png"));
    Reset->setToolTip("Reset Image");
    Reset->setIcon(QIcon(":res/ImageEditorDlg/reset.png"));
    
    // set check box text
    Ratio->setText("Set Cropping Rectangle Ratio Fix to Screen");
    
    setup_WidgetsLayout();
    make_Connection();
}
void ImageEditorDlg::make_Connection()
{
    connect(RotateL90, &QPushButton::released, ie, &ImageEditor::rotateL90);
    connect(RotateR90, &QPushButton::released, ie, &ImageEditor::rotateR90);
    connect(RotateCroppingBox, &QPushButton::released, ie, &ImageEditor::rotateCroppingRect);
    connect(MirrorH, &QPushButton::released, ie, &ImageEditor::mirrorHori);
    connect(MirrorV, &QPushButton::released, ie, &ImageEditor::mirrorVrti);
    connect(Undo, &QPushButton::released, ie, &ImageEditor::undo);
    connect(Crop, &QPushButton::released, ie, &ImageEditor::crop);
    connect(Reset, &QPushButton::released, ie, &ImageEditor::reset);
    connect(Ratio, &QCheckBox::stateChanged, this, &ImageEditorDlg::Toggled);
}
void ImageEditorDlg::setup_WidgetsLayout()
{
    // init layouts
    QVBoxLayout *m_LayoutV = new QVBoxLayout(this);
    QHBoxLayout *m_LayoutH = new QHBoxLayout(this);
    
    // use m_LayoutV as base layout
    m_LayoutV->setAlignment(m_LayoutV, Qt::AlignCenter);
    m_LayoutV->addWidget(ie); // m_LayoutV[0] => ie
    m_LayoutV->insertLayout(1, m_LayoutH); // m_LayoutV[1] => m_LayoutH
    
    // use m_LayoutH as sub layout of m_LayoutV
    // add buttons & checkbox into it
    m_LayoutH->addWidget(RotateL90);
    m_LayoutH->addWidget(RotateR90);
    m_LayoutH->addWidget(RotateCroppingBox);
    m_LayoutH->addWidget(MirrorH);
    m_LayoutH->addWidget(MirrorV);
    m_LayoutH->addWidget(Undo);
    m_LayoutH->addWidget(Crop);
    m_LayoutH->addWidget(Reset);
    m_LayoutH->addWidget(Ratio);
}




// custom widgets that ignore keyPressEvent

My_PushButton::My_PushButton(QWidget* parent)
    : QPushButton(parent)
{}
void My_PushButton::keyPressEvent(QKeyEvent *e)
{
    e->ignore();
}


My_CheckBox::My_CheckBox(QWidget* parent)
    : QCheckBox(parent)
{}
void My_CheckBox::keyPressEvent(QKeyEvent *e)
{
    e->ignore();
}
