
// implementation of ImageEditor
#include "ImageEditor.h"

// Qt Dependency Libs
#include <QPainter>
#include <QPainterPath>


// ====================== Public Functions ======================

QSize get_screen_res()
{
    // get primary screen resolution after zoom in/out
    int CX = GetSystemMetrics(SM_CXSCREEN);
    int CY = GetSystemMetrics(SM_CYSCREEN);
    
    return QSize(CX, CY);
}
QSizeF get_screen_resF()
{
    // get primary screen resolution after zoom in/out
    double CX = static_cast<double>(GetSystemMetrics(SM_CXSCREEN));
    double CY = static_cast<double>(GetSystemMetrics(SM_CYSCREEN));
    
    return QSizeF(CX, CY);
}

// ====================== Public Functions End ======================





// ====================== PairRectF ======================

// default constructor
PairRectF::PairRectF()
    : m_Original(QRectF()),
    m_Scaled(QRectF()),
    m_ScaleFactor(1.0),
    m_Displacement(0.0, 0.0)
{}
// parametric constructor
PairRectF::PairRectF(
    const QRectF& original_rectf,
    const double& scale_factor,
    const QPointF& center_displacement)
    : m_Original(QRectF()),
    m_Scaled(QRectF()),
    m_ScaleFactor(scale_factor),
    m_Displacement(center_displacement)
{
    setOriginal(original_rectf);
}
PairRectF::PairRectF(
    const QRect& original_rectf,
    const double& scale_factor,
    const QPointF& center_displacement)
    : m_Original(QRectF()),
    m_Scaled(QRectF()),
    m_ScaleFactor(scale_factor),
    m_Displacement(center_displacement)
{
    setOriginal(QRectF(original_rectf));
}

// modifthisr

// modify original QRectF
// scaled QRectF will change with it 
void PairRectF::setOriginal(const QRectF& ori)
{
    setOriX(ori.x());
    setOriY(ori.y());
    setOriWidth(ori.width());
    setOriHeight(ori.height());
}
void PairRectF::setOriX(const double& x)
{
    m_Original.setX(x);
    m_Scaled.setX((x * m_ScaleFactor) - m_Displacement.x());
}
void PairRectF::setOriY(const double& y)
{
    m_Original.setY(y);
    m_Scaled.setY((y * m_ScaleFactor) - m_Displacement.y());
}
void PairRectF::setOriWidth(const double& w)
{
    m_Original.setWidth(w);
    m_Scaled.setWidth(w * m_ScaleFactor);
}
void PairRectF::setOriHeight(const double& h)
{
    m_Original.setHeight(h);
    m_Scaled.setHeight(h * m_ScaleFactor);
}

// modify scaled QRectF
// original QRectF will change with it 
void PairRectF::setScaled(const QRectF& scal)
{
    setScalX(scal.x());
    setScalY(scal.y());
    setScalWidth(scal.width());
    setScalHeight(scal.height());
}
void PairRectF::setScalX(const double& x)
{
    m_Scaled.setX(x);
    m_Original.setX((x + m_Displacement.x()) / m_ScaleFactor);
}
void PairRectF::setScalY(const double& y)
{
    m_Scaled.setY(y);
    m_Original.setY((y + m_Displacement.y()) / m_ScaleFactor);
}
void PairRectF::setScalWidth(const double& w)
{
    m_Scaled.setWidth(w);
    m_Original.setWidth(w / m_ScaleFactor);
}
void PairRectF::setScalHeight(const double& h)
{
    m_Scaled.setHeight(h);
    m_Original.setHeight(h / m_ScaleFactor);
}

// change scale factor, scaled QRectF will change
void PairRectF::setScaleFactor(const double& scale_factor)
{
    m_ScaleFactor = scale_factor;
    setOriginal(m_Original);
}
// change displacement, scaled QRectF will change
void PairRectF::setDisplacement(const QPointF& displacement)
{
    m_Displacement = displacement;
    setOriginal(m_Original);
}

// rotate both Original & Scaled QRectF 90 deg
// switch status between landscape & portrait
void PairRectF::rotate()
{
    // only rotate width & height
    QRectF temp(
        m_Original.x(),
        m_Original.y(),
        m_Original.height(),
        m_Original.width()
    );
    this->setOriginal(temp);
}


// getter

// original
const QRectF PairRectF::Original() const
{
    return m_Original;
}
const double PairRectF::OriX() const
{
    return m_Original.x();
}
const double PairRectF::OriY() const
{
    return m_Original.y();
}
const double PairRectF::OriWidth() const
{
    return m_Original.width();
}
const double PairRectF::OriHeight() const
{
    return m_Original.height();
}
// scaled
const QRectF PairRectF::Scaled() const
{
    return m_Scaled;
}
const double PairRectF::ScalX() const
{
    return m_Scaled.x();
}
const double PairRectF::ScalY() const
{
    return m_Scaled.y();
}
const double PairRectF::ScalWidth() const
{
    return m_Scaled.width();
}
const double PairRectF::ScalHeight() const
{
    return m_Scaled.height();
}
// scale factor
const double PairRectF::ScaleFactor() const
{
    return m_ScaleFactor;
}
// displacement
const QPointF PairRectF::Displacement() const
{
    return m_Displacement;
}

// Other status
const bool PairRectF::isNull() const
{
    return (
        m_Original.isNull() &&
        m_Scaled.isNull()
    );
}

// ====================== PairRectF End ======================





// ====================== CursorPosition ======================

// default constructor
CursorPosition::CursorPosition()
    : m_NearDefinition(10.0),
    m_CroppingRect(QRectF()),
    m_Cursor(QPointF()),
    m_CursorPositionType(PositionType::Unknown_Outside)
{}
// parametric constructor
CursorPosition::CursorPosition(
    const QRectF& cropping_rect,
    const QPointF& cursor,
    const double& define_near
)
    : m_NearDefinition(define_near),
    m_CroppingRect(cropping_rect),
    m_Cursor(cursor),
    m_CursorPositionType(PositionType::Unknown_Outside)
{
    calc_relativity();
}

// update functions
void CursorPosition::updateCroppingRect(const QRectF& new_cropping_rect)
{
    m_CroppingRect = new_cropping_rect;
    calc_relativity();
}
void CursorPosition::updateCursor(const QPointF& new_cursor)
{
    m_Cursor = new_cursor;
    calc_relativity();
}

// getters
PositionType CursorPosition::getPosition()
{
    return m_CursorPositionType;
}
const PositionType CursorPosition::getPosition() const
{
    return m_CursorPositionType;
}

// private: // helper functions
// calculate position relativity 
// of CroppingRect and Cursor
// and then update m_CursorPositionType
void CursorPosition::calc_relativity()
{
    // cursor coordinate info
    double cur_x = static_cast<double>(m_Cursor.x());
    double cur_y = static_cast<double>(m_Cursor.y());
    // cropping rect top left coordinate info
    double rectTL_x = static_cast<double>(m_CroppingRect.topLeft().x());
    double rectTL_y = static_cast<double>(m_CroppingRect.topLeft().y());
    // cropping rect bottom right coordinate info
    double rectBR_x = static_cast<double>(m_CroppingRect.bottomRight().x());
    double rectBR_y = static_cast<double>(m_CroppingRect.bottomRight().y());
    
    // determine whether cursor is within a range of
    // within range of rectTL_x and rectBR_x 
    bool withinRangeX = (
        cur_x >= rectTL_x && // from left
        cur_x <= rectBR_x // to right
    );
    // within range of rectBR_y and rectTL_y
    bool withinRangeY = (
        cur_y >= rectTL_y && // from top
        cur_y <= rectBR_y // to bottom
    );
    // within diagonal sections:
    // top left, top right, bottom left, bottom right
    bool withinDia = !withinRangeX && !withinRangeY;
    
    // check relationship between cursor and cropping rect
    bool nearTop = std::fabs(cur_y - rectTL_y) <= m_NearDefinition;
    bool nearLeft = std::fabs(cur_x - rectTL_x) <= m_NearDefinition;
    bool nearBottom = std::fabs(cur_y - rectBR_y) <= m_NearDefinition;
    bool nearRight = std::fabs(cur_x - rectBR_x) <= m_NearDefinition;
    bool notNear = !nearTop && !nearLeft && !nearBottom && !nearRight;
    
    // determine cursor's PositionType
    // whether cursor is inside cropping rect & not near to any side
    if (m_CroppingRect.contains(m_Cursor) && notNear)
        m_CursorPositionType = PositionType::Inside;
    // whether cursor is near vertexes of cropping rect
    else if (withinDia && nearTop && nearLeft)
        m_CursorPositionType = PositionType::TopLeft;
    else if (withinDia && nearTop && nearRight)
        m_CursorPositionType = PositionType::TopRight;
    else if (withinDia && nearBottom && nearLeft)
        m_CursorPositionType = PositionType::BottomLeft;
    else if (withinDia && nearBottom && nearRight)
        m_CursorPositionType = PositionType::BottomRight;
    // whether cursor is near sides of cropping rect
    else if (withinRangeY && nearLeft)
        m_CursorPositionType = PositionType::Left;
    else if (withinRangeY && nearRight)
        m_CursorPositionType = PositionType::Right;
    else if (withinRangeX && nearTop)
        m_CursorPositionType = PositionType::Top;
    else if (withinRangeX && nearBottom)
        m_CursorPositionType = PositionType::Bottom;
    // other situation
    else
        m_CursorPositionType = PositionType::Unknown_Outside;
}

// ====================== CursorPosition End ======================





// ====================== ImageEditor ======================

// default constructor
ImageEditor::ImageEditor(QWidget* parent)
    : QWidget(parent),
    // image buffers
    m_SrcImagePath(""),
    p_ImageReader(nullptr),
    m_ImageScale(QSize(0, 0)),
    m_CurrImage(QImage()),
    m_PrevImage(QImage()),
    m_ScaledImage(QImage()),
    m_CropWHRatio(1.0),
    m_IsFixWHRatio(false),
    // cropping rect proporties
    m_ImageRect(PairRectF()),
    m_LastCursorPos(QPointF()),
    m_IsMousePressingDown(false),
    m_CursorPosition(CursorPosition()),
    m_IsPositionTypeLocked(false),
    m_LastPositionType(PositionType::Unknown_Outside),
    // other widget
    m_CroppingRectColor(QColor(Qt::magenta)),
    m_WidgetBackgroundColor(QColor(Qt::gray)),
    m_IsFirstTimePainting(true)
{
    this->setMinimumSize(QSize(300, 300));
    // allow widget to receive QMouseEvent
    this->setMouseTracking(true);
    // allow widget to receive QKeyEvent
    this->setFocusPolicy(Qt::StrongFocus);
}

// basic widget proportthiss setting

// set/reset widget image
void ImageEditor::setImage(const QString& img_path)
{
    // init m_ImageScale with a temp value for
    // 1st time enter this function
    if (m_ImageScale.isNull()) {
        m_ImageScale = 
            QSize(
                this->rect().width(),
                this->rect().height()
            );
    }
    
    // read image from img_path
    m_SrcImagePath = img_path;
    p_ImageReader = setImageReader(m_SrcImagePath);
    if (p_ImageReader->canRead()) {
        m_CurrImage = p_ImageReader->read();
        m_PrevImage = m_CurrImage;
    } else 
        throw std::invalid_argument("Cannot Open Image File\n");
    delete p_ImageReader;
    
    // calculate scale factor
    double scale_factor = update_ScaleFactor(m_CurrImage.rect());
    // apply changes to m_ImageRect
    m_ImageRect.setScaleFactor(scale_factor);
    reset_ImageRectF(m_CurrImage.rect());
    // update cropping rect in m_CursorPosition
    m_CursorPosition.updateCroppingRect(m_ImageRect.Scaled());
    update_PaintingImage();
    
    // update widget, draw image
    this->update();
}

// get image proportthiss
const QString ImageEditor::getSrcImagePath() const
{
    return m_SrcImagePath;
}
// return image scale factor
// widget will display an image
// scaled by this factor
const double ImageEditor::getImageScaleFactor() const
{
    return m_ImageRect.ScaleFactor();
}
const QImage ImageEditor::getImage() const
{
    return m_CurrImage;
}
// prop
const double ImageEditor::getCroppingWHRatio() const
{
    return m_CropWHRatio;
}
const bool ImageEditor::isFixWHRatio() const
{
    return m_IsFixWHRatio;
}
// get cropping rect & painting rect
const PairRectF ImageEditor::getImageRect() const
{
    return m_ImageRect;
}


// setting Width Height Ratio of cropper
void ImageEditor::setWHRatio(const QSizeF& width_height)
{
    m_CropWHRatio = 
        static_cast<double>(width_height.width()) / 
        static_cast<double>(width_height.height());
}
void ImageEditor::setFixWHRatio2Screen(const bool& flag)
{
    m_IsFixWHRatio = flag;
    if (m_IsFixWHRatio) {
        setWHRatio(get_screen_resF());
        // update m_ImageRect with new ratio as well
        double scale_factor = m_ImageRect.ScaleFactor();
        // calculate current painted image rect
        QRectF img_rect = QRectF(m_CurrImage.rect());
        // set new cropping rect to middle of
        // current painted image rect
        img_rect.setWidth(img_rect.width() * scale_factor / 3.0);
        img_rect.setHeight(img_rect.height() * scale_factor / 3.0);
        bool is_ImageRect_outOfRange = (
            m_ImageRect.ScalX() < 0 || // negative x
            m_ImageRect.ScalY() < 0 || // negative y
            m_ImageRect.ScalX() > m_ImageRect.ScalWidth() || // x > width
            m_ImageRect.ScalY() > m_ImageRect.ScalHeight() // y > height
        );
        if (is_ImageRect_outOfRange) {
            img_rect.setX(img_rect.width());
            img_rect.setY(img_rect.height());
        } else {
            img_rect.setX(m_ImageRect.ScalX());
            img_rect.setY(m_ImageRect.ScalY());
        }
        // adjust img_rect to screen WHRatio
        QRectF temp = img_rect;
        if (temp.width() >= temp.height())
            temp.setWidth(temp.height() * m_CropWHRatio);
        else // temp.width() < temp.height()
            temp.setHeight(temp.width() / m_CropWHRatio);
        // update m_ImageRect & redraw it
        m_ImageRect.setOriginal(temp);
        
        this->update();
    }
}



// public slots: // editing functions

// crop current image
void ImageEditor::crop()
{
    // crop current image
    m_PrevImage = m_CurrImage;
    // ignore out of range part of cropping rect
    QRectF cropping_rect = m_ImageRect.Original();
    QPointF topLeft = cropping_rect.topLeft();
    QPointF bottomRight = cropping_rect.bottomRight();
    double buff_w = cropping_rect.width();
    double buff_h = cropping_rect.height();
    double img_w = m_CurrImage.rect().width();
    double img_h = m_CurrImage.rect().height();
    // x axis out of range
    if (topLeft.x() < 0) { // top left x being negative
        buff_w = buff_w + cropping_rect.x();
        cropping_rect.setX(0);
    } // bottom right x greater than image width
    if (bottomRight.x() > 0 && bottomRight.x() > img_w)  {
        double diff = bottomRight.x() - img_w;
        buff_w = buff_w - diff;
    }
    // y axis out of range
    if (topLeft.y() < 0) { // top left y being negative
        buff_h = buff_h + topLeft.y();
        cropping_rect.setY(0);
    } // bottom right y greater than image height
    if (bottomRight.y() > 0 && bottomRight.y() > img_h) {
        double diff = bottomRight.y() - img_h;
        buff_h = buff_h - diff;
    }
    cropping_rect.setWidth(buff_w);
    cropping_rect.setHeight(buff_h);
    // crop
    m_CurrImage = m_CurrImage.copy(cropping_rect.toRect());
    update_PaintingImage();
    // update m_ImageRect    
    m_ImageRect.setScaleFactor(update_ScaleFactor(m_CurrImage.rect()));
    m_ImageRect.setOriginal(QRectF(m_CurrImage.rect()));
    update_CropWHRatio();
    // update cropping rect in m_CursorPosition
    m_CursorPosition.updateCroppingRect(m_ImageRect.Scaled());
    
    this->update();
}
// Image rotation
void ImageEditor::rotateL90()
{
    int w = m_CurrImage.width();
    int h = m_CurrImage.height();
    QImage buff(h, w, m_CurrImage.format());
    reset_ImageRectF(buff.rect());
    
    QRgb pix_val;
    // loop through whole image 
    // and place each pixel to buff
    for (int i = 0; i < w; i++) { // width
        for (int j = 0; j < h; j++) { // height
            pix_val = m_CurrImage.pixel(i, j);
            buff.setPixel(j, w-i-1, pix_val);
        }
    }
    // update image
    m_PrevImage = m_CurrImage;
    m_CurrImage = buff;
    update_PaintingImage();
    this->update();
}
void ImageEditor::rotateR90()
{
    int w = m_CurrImage.width();
    int h = m_CurrImage.height();
    QImage buff(h, w, m_CurrImage.format());
    reset_ImageRectF(buff.rect());
    QRgb pix_val;
    // loop through whole image 
    // and place each pixel to buff
    for (int i = 0; i < w; i++) { // width
        for (int j = 0; j < h; j++) { // height
            pix_val = m_CurrImage.pixel(i, j);
            buff.setPixel(h-j-1, i, pix_val);
        }
    }
    // update image
    m_PrevImage = m_CurrImage;
    m_CurrImage = buff;
    update_PaintingImage();
    this->update();
}
void ImageEditor::mirrorHori()
{
    m_PrevImage = m_CurrImage;
    m_CurrImage.mirror(true, false);
    update_PaintingImage();
    this->update();
}
void ImageEditor::mirrorVrti()
{
    m_PrevImage = m_CurrImage;
    m_CurrImage.mirror(false, true);
    update_PaintingImage();
    this->update();
}
// rotate only cropping rect, change 
// m_ImageRect status between Landscape & Portrait
void ImageEditor::rotateCroppingRect()
{
    m_ImageRect.rotate();
    QSizeF temp(
        m_ImageRect.OriHeight(),
        m_ImageRect.OriWidth());
    this->setWHRatio(temp);
    this->update();
}
// one operation forward / backward
void ImageEditor::undo()
{
    QImage buff = m_CurrImage;
    m_CurrImage = m_PrevImage;
    m_PrevImage = buff;
    update_PaintingImage();
    reset_ImageRectF(m_CurrImage.rect());
    this->update();
}
// saving file
void ImageEditor::save()
{
    m_CurrImage.save(m_SrcImagePath, "BMP", 100);
}
void ImageEditor::saveAs(const QString& new_filepath)
{
    m_SrcImagePath = new_filepath;
    m_CurrImage.save(m_SrcImagePath, "BMP", 100);
}
// reset to original image file
// this function will reload original source image file
void ImageEditor::reset()
{
    try {
        this->setImage(m_SrcImagePath);
        this->update();
    } catch(std::exception& err) {
        throw err;
    }
}

// moving cropping rect base on m_CurrImage pixels
// pix_difference is moving delta (x, y) of this move
void ImageEditor::moveCroppingRectF(const QPointF& pix_difference)
{
    QRectF temp = m_ImageRect.Original();
    temp.moveCenter(temp.center()+pix_difference);
    m_ImageRect.setOriginal(temp);
    
    this->update();
}
void ImageEditor::moveCroppingRect(const QPoint& pix_difference)
{
    this->moveCroppingRectF(QPointF(pix_difference));
}
// move to center of widget & reset its size
void ImageEditor::resetCroppingRect()
{
    reset_ImageRectF(m_CurrImage.rect());
    
    this->update();
}




// protected: // event handling functions
void ImageEditor::paintEvent(QPaintEvent* event)
{
    this->QWidget::paintEvent(event);
    
    // do not paint widget if do not have image
    if (m_CurrImage.isNull())
        return;
    
    // init painter
    QPainter painter(this);
    
    // paint widget background
    painter.fillRect(this->rect(), m_WidgetBackgroundColor);
    
    // init variables when 1st time draw dlg
    if (m_IsFirstTimePainting) {
        // update m_ImageScale to real widget size
        m_ImageScale = QSize(
            this->rect().width(),
            this->rect().height()
        );
        
        // update m_ImageRect rect & scale factor
        reset_ImageRectF(m_CurrImage.rect());
        
        // update m_ScaledImage & m_ImageRect Displacement
        update_PaintingImage();
        
        // update cropping rect in m_CursorPosition
        m_CursorPosition.updateCroppingRect(m_ImageRect.Scaled());
        
        // only run this section once
        m_IsFirstTimePainting = false;
    }
    
    // draw m_ScaledImage on center of widget base on widget rect
    if (this->size().height() == m_ScaledImage.height()) {
        painter.drawImage(
            (this->width() - m_ScaledImage.width()) / 2, 0,
            m_ScaledImage
        );
    } else {
        painter.drawImage(
            0, (this->height() - m_ScaledImage.height()) / 2,
            m_ScaledImage
        );
    }
    
    // draw cropping rect
    QPainterPath ppath;
    if (m_ImageRect.isNull())
        reset_ImageRectF(m_CurrImage.rect());
    
    // draw cropping transparent shadow
    ppath.addRect(m_ImageRect.Scaled());
    ppath.addRect(this->rect());
    painter.setBrush(QBrush(QColor(0,0,0,120)));
    painter.setPen(Qt::transparent);
    painter.drawPath(ppath);
    
    // draw cropping boarder
    painter.setPen(m_CroppingRectColor);
    painter.setBrush(QBrush(Qt::transparent));
    painter.drawRect(m_ImageRect.Scaled());
    
    // get cropping boarder points
    // leave a few pixels off for drawing dot later
    QPoint TopLeft = QPoint(
        m_ImageRect.Scaled().topLeft().x() - 2,
        m_ImageRect.Scaled().topLeft().y() - 2
    );
    QPoint Center = QPoint(
        m_ImageRect.Scaled().center().x() - 3,
        m_ImageRect.Scaled().center().y() - 3
    );
    QPoint BottomRight = QPoint(
        m_ImageRect.Scaled().bottomRight().x() - 2,
        m_ImageRect.Scaled().bottomRight().y() - 2
    );
    // set point size
    const QSize pointSize(6, 6);
    // all points need to draw
    QVector<QRect> points;
    points
        // left vertical line
        << QRect(TopLeft, pointSize)
        << QRect(QPoint(TopLeft.x(), Center.y()), pointSize)
        << QRect(QPoint(TopLeft.x(), BottomRight.y()), pointSize)
        // center vertical line
        << QRect(QPoint(Center.x(), TopLeft.y()), pointSize)
        << QRect(Center, pointSize)
        << QRect(QPoint(Center.x(), BottomRight.y()), pointSize)
        // right vertical line
        << QRect(QPoint(BottomRight.x(), TopLeft.y()), pointSize)
        << QRect(QPoint(BottomRight.x(), Center.y()), pointSize)
        << QRect(BottomRight, pointSize)
    ;
    // draw points
    painter.setBrush(QBrush(m_CroppingRectColor));
    painter.drawRects(points);
    
    // draw dash lines cross over the middle of rectangle
    QPen dashPen(m_CroppingRectColor);
    dashPen.setStyle(Qt::DashLine);
    painter.setPen(dashPen);
    // draw lines
    painter.drawLine(
        // from this point
        QPoint(
            m_ImageRect.Scaled().center().x(),
            m_ImageRect.Scaled().top()
            ),
        // to this point
        QPoint(
            m_ImageRect.Scaled().center().x(),
            m_ImageRect.Scaled().bottom()
            )
    );
    painter.drawLine(
        // from this point
        QPoint(
            m_ImageRect.Scaled().left(),
            m_ImageRect.Scaled().center().y()
            ),
        // to this point
        QPoint(
            m_ImageRect.Scaled().right(),
            m_ImageRect.Scaled().center().y()
            )
    );
    
    // end painting
    painter.end();
}
void ImageEditor::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        m_IsMousePressingDown = true;
        // lock cursor PositionType for future operation
        m_IsPositionTypeLocked = true;
    }
    update_CursorIcon();
}
void ImageEditor::mouseMoveEvent(QMouseEvent* event)
{
    // update cursor
    m_CursorPosition.updateCroppingRect(m_ImageRect.Scaled());
    m_CursorPosition.updateCursor(QPointF(event->pos()));
    
    // update m_ImageRect if needed
    PositionType curr_pos = m_LastPositionType;
    if (!m_IsPositionTypeLocked) {
        curr_pos = m_CursorPosition.getPosition();
        m_LastPositionType = curr_pos;
        update_CursorIcon();
    }
    if (m_IsMousePressingDown && // mouse pressing down
        // cursor inside effective area
        curr_pos != PositionType::Unknown_Outside) 
    {
        double dx = event->pos().x() - m_LastCursorPos.x();
        double dy = event->pos().y() - m_LastCursorPos.y();
        QRectF new_cropping_rect = m_ImageRect.Scaled();
        QPointF topLeft = new_cropping_rect.topLeft();
        QPointF bottomRight = new_cropping_rect.bottomRight();
        // modify m_ImageRect base on current cursor position & dx, dy
        if (curr_pos == PositionType::Inside) {
            new_cropping_rect.moveTopLeft(QPointF(
                topLeft.x()+dx,
                topLeft.y()+dy
            ));
        } else if (!m_IsFixWHRatio) {
            mouseMove_WithCustomCropWHRatio(
                curr_pos,
                dx, dy,
                topLeft, bottomRight,
                new_cropping_rect);
        } else { // m_IsFixWHRatio == true
            mouseMove_WithFixedCropWHRatio(
                curr_pos,
                dx, dy,
                topLeft, bottomRight,
                new_cropping_rect);
        }
        
        // prevent width && height become negative
        if (new_cropping_rect.width() <= 3 ||
            new_cropping_rect.height() <= 3) 
        {
            new_cropping_rect.setTopLeft(topLeft);
            new_cropping_rect.setBottomRight(bottomRight);
        }
        
        // update m_ImageRect & repaint widget
        m_ImageRect.setScaled(new_cropping_rect);
        update_CropWHRatio();
        this->update();
    }
    // store current cursor position as well
    m_LastCursorPos = event->pos();
}
void ImageEditor::mouseReleaseEvent(QMouseEvent* event)
{
    // set mouse pressing down to false
    m_IsMousePressingDown = false;
    // unlock PositionType & reset locked PositionType
    m_IsPositionTypeLocked = false;
    m_LastPositionType = PositionType::Unknown_Outside;
    // update current cursor icon
    update_CursorIcon();
}


// private: // helper functions

void ImageEditor::update_PaintingImage()
{
    // generate new image for painting
    m_ScaledImage = 
        m_CurrImage.scaled(
            m_ImageScale,
            Qt::KeepAspectRatio,
            Qt::FastTransformation
        );
    
    update_Displacement();
}
void ImageEditor::update_Displacement()
{
    // update m_ImageRect Displacement base on
    // widget rect & m_ScaledImage rect
    QPoint displacement;
    if (this->size().height() == m_ScaledImage.height()) {
        displacement = QPoint(
            (this->width() - m_ScaledImage.width()) / 2 * -1,
            0
        );
    } else {
        displacement = QPoint(
            0,
            (this->height() - m_ScaledImage.height()) / 2 * -1
        );
    }
    
    m_ImageRect.setDisplacement(displacement);
}
void ImageEditor::reset_ImageRectF(const QRect& original_img_rect)
{
    QRect loc_rect = original_img_rect;
    if (m_IsFixWHRatio) {
        if (loc_rect.width() <= loc_rect.height())
            loc_rect.setHeight(loc_rect.width()/m_CropWHRatio);
        else // loc_rect.width() > loc_rect.height()
            loc_rect.setWidth(loc_rect.height()*m_CropWHRatio);
    }
    QRect buff = loc_rect;
    double wC = buff.width();
    double hC = buff.height();
    // initial buff to be 1/3 of loc_rect &
    // so it sit on center of loc_rect
    buff.setX(wC/3.0);
    buff.setY(hC/3.0);
    buff.setWidth(wC/3.0);
    buff.setHeight(hC/3.0);
    // update scale factor
    double scale_factor = update_ScaleFactor(loc_rect);
    // apply changes to m_ImageRect
    m_ImageRect.setScaleFactor(scale_factor);
    m_ImageRect.setOriginal(QRectF(buff));
    update_CropWHRatio();
}
const double ImageEditor::update_ScaleFactor(
    const QRect& new_scale_factor) const
{
    return update_ScaleFactorF(QRectF(new_scale_factor));
}
const double ImageEditor::update_ScaleFactorF(
    const QRectF& new_scale_factor) const
{
    double w = static_cast<double>(new_scale_factor.width());
    double h = static_cast<double>(new_scale_factor.height());
    
    // update scale factor
    double scale_factor = 0.0;
    
    // compute scale_factor with img width & height
    // also compute height & width from scale_factors
    double factor_w = m_ImageScale.width() / w;
    double scaled_h = h * factor_w;
    double factor_h = m_ImageScale.height() / h;
    double scaled_w = w * factor_h;
    
    // compare scaled height & width w/ screen size
    // and then pick the scale factor from a 
    // scaled height/width smaller than screen height/width
    if (scaled_h <= this->rect().height())
        scale_factor = factor_w; // pick the scale_factor used to compute scaled_h
    else if (scaled_w <= this->rect().width())
        scale_factor = factor_h; // pick the scale_factor used to compute scaled_w
    // scaled_h > m_ImageScale.height() || scaled_w > m_ImageScale.width()
    // is impossible to happen
    
    return scale_factor;
}

void ImageEditor::update_CropWHRatio()
{
    m_CropWHRatio = m_ImageRect.OriWidth() / m_ImageRect.OriHeight();
}

QImageReader* ImageEditor::setImageReader(const QString& filename)
{
    QImageReader *buff = new QImageReader();
    buff->setDecideFormatFromContent(true);
    buff->setFileName(filename);
    return std::move(buff);
}

// update cursor icon base on cursor position
void ImageEditor::update_CursorIcon()
{
    QCursor cursor_icon;
    // switch cursor icon base on cursor position
    switch (m_CursorPosition.getPosition())
    {
    // cursor icon: backward slash with arrow
    case PositionType::TopLeft:
    case PositionType::BottomRight:
        cursor_icon = QCursor(Qt::SizeFDiagCursor);
        break;
    
    // cursor icon: / forward slash with arrow
    case PositionType::TopRight:
    case PositionType::BottomLeft:
        cursor_icon = QCursor(Qt::SizeBDiagCursor);
        break;
    
    // cursor icon: | vertical line with arrow
    case PositionType::Top:
    case PositionType::Bottom:
        cursor_icon = QCursor(Qt::SizeVerCursor);
        break;
    
    // cursor icon: - horizontal line with arrow
    case PositionType::Left:
    case PositionType::Right:
        cursor_icon = QCursor(Qt::SizeHorCursor);
        break;
    
    // cursor icon: hand
    case PositionType::Inside:
        if (m_IsMousePressingDown) // close hand
            cursor_icon = QCursor(Qt::ClosedHandCursor);
        else // m_IsMousePressingDown == false; open hand
            cursor_icon = QCursor(Qt::OpenHandCursor);
        break;
    
    // cursor icon: normal Arrow icon
    case PositionType::Unknown_Outside:
    default:
        cursor_icon = QCursor(Qt::ArrowCursor);
        break;
    }
    // update cursor
    this->setCursor(cursor_icon);
}

// handle Cropping Rect change during mouse move event
void ImageEditor::mouseMove_WithCustomCropWHRatio(
    const PositionType& curr_pos,
    const double& dx,
    const double& dy,
    const QPointF& topLeft,
    const QPointF& bottomRight,
    QRectF& new_cropping_rect) 
{
    switch (curr_pos)
    {
    // 3 Positions on the top
    case PositionType::TopLeft:
        new_cropping_rect.setTopLeft(QPointF(
            topLeft.x()+dx,
            topLeft.y()+dy
        ));
        break;
    case PositionType::Top:
        new_cropping_rect.setTop(topLeft.y()+dy);
        break;
    case PositionType::TopRight:
        new_cropping_rect.setTopRight(QPointF(
            bottomRight.x()+dx,
            topLeft.y()+dy
        ));
        break;
    // 2 Positions in middle
    case PositionType::Left:
        new_cropping_rect.setLeft(topLeft.x()+dx);
        break;
    case PositionType::Right:
        new_cropping_rect.setRight(bottomRight.x()+dx);
        break;
    // 3 Positions in bottom
    case PositionType::BottomLeft:
        new_cropping_rect.setBottomLeft(QPointF(
            topLeft.x()+dx,
            bottomRight.y()+dy
        ));
        break;
    case PositionType::Bottom:
        new_cropping_rect.setBottom(
            bottomRight.y()+dy
        );
        break;
    case PositionType::BottomRight:
        new_cropping_rect.setBottomRight(QPointF(
            bottomRight.x()+dx,
            bottomRight.y()+dy
        ));
        break;
    };
}
// handle Cropping Rect change during mouse move event
void ImageEditor::mouseMove_WithFixedCropWHRatio(
    const PositionType& curr_pos,
    const double& dx,
    const double& dy,
    const QPointF& topLeft,
    const QPointF& bottomRight,
    QRectF& new_cropping_rect)
{
    double new_dx = 0.0;
    double new_dy = 0.0;
    // use greater value to calulcate smaller value
    // in all vertexes
    switch (curr_pos)
    {
    // move topLeft, bottomRight stay fix
    case PositionType::TopLeft:
        if (std::fabs(dx) >= std::fabs(dy)) {
            new_dx = dx;
            new_dy = dx / m_CropWHRatio;
        } else {
            new_dx = dy * m_CropWHRatio;
            new_dy = dy;
        }
        new_cropping_rect.setTopLeft(QPointF(
            topLeft.x()+new_dx,
            topLeft.y()+new_dy
        ));
        break;
    // move bottomRight, topLeft stay fix
    case PositionType::BottomRight:
        if (std::fabs(dx) >= std::fabs(dy)) {
            new_dx = dx;
            new_dy = dx / m_CropWHRatio;
        } else {
            new_dx = dy * m_CropWHRatio;
            new_dy = dy;
        }
        new_cropping_rect.setBottomRight(QPointF(
            bottomRight.x()+new_dx,
            bottomRight.y()+new_dy
        ));
        break;
    // move topRight, bottomLeft stay fix
    case PositionType::Top:
        new_dx = -1 * dy * m_CropWHRatio;
        new_cropping_rect.setTopRight(QPointF(
            bottomRight.x()+new_dx,
            topLeft.y()+dy
        ));
        break;
    case PositionType::TopRight:
        if (std::fabs(dx) >= std::fabs(dy)) {
            new_dx = dx;
            new_dy = -1 * dx / m_CropWHRatio;
        } else {
            new_dx = -1 * dy * m_CropWHRatio;
            new_dy = dy;
        }
        new_cropping_rect.setTopRight(QPointF(
            bottomRight.x()+new_dx,
            topLeft.y()+new_dy
        ));
        break;
    case PositionType::Right:
        new_dy = -1 * dx / m_CropWHRatio;
        new_cropping_rect.setTopRight(QPointF(
            bottomRight.x()+dx,
            topLeft.y()+new_dy
        ));
        break;
    // move bottomLeft, topRight stay fix
    case PositionType::Bottom:
        new_dx = -1 * dy * m_CropWHRatio;
        new_cropping_rect.setBottomLeft(QPointF(
            topLeft.x()+new_dx,
            bottomRight.y()+dy
        ));
        break;
    case PositionType::Left:
        new_dy = -1 * dx / m_CropWHRatio;
        new_cropping_rect.setBottomLeft(QPointF(
            topLeft.x()+dx,
            bottomRight.y()+new_dy
        ));
        break;
    case PositionType::BottomLeft:
        if (std::fabs(dx) >= std::fabs(dy)) {
            new_dx = dx;
            new_dy = -1 * dx / m_CropWHRatio;
        } else {
            new_dx = -1 * dy * m_CropWHRatio;
            new_dy = dy;
        }
        new_cropping_rect.setBottomLeft(QPointF(
            topLeft.x()+new_dx,
            bottomRight.y()+new_dy
        ));
        break;
    };
}

// ====================== ImageEditor End ======================
