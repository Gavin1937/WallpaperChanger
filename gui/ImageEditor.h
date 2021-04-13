
// declaration of ImageEditor
#pragma once

// Qt Lib
#include <QApplication>
#include <QDialog>
#include <QImage>
#include <QImageReader>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QRect>
#include <QWidget>

// C++ STL
#include <cmath>
#include <stdexcept>

// WIN32 API
#include <windows.h>

// public declaration
class PairRectF;
class ImageEditor;
enum PositionType {
    Unknown_Outside = 0,
    TopLeft,     Top,     TopRight,
    Left,        Inside,  Right,
    BottomLeft,  Bottom,  BottomRight
};
class CursorPosition;

// public functions
QSize get_screen_res();
QSizeF get_screen_resF();



// A pair of similar QRectF
// That is, m_Scaled will change as
// m_Original change with m_ScaleFactor & m_Displacement
class PairRectF
{
public:
    // default constructor
    PairRectF();
    // parametric constructor
    PairRectF(
        const QRectF& original_rectf,
        const double& scale_factor,
        const QPointF& center_displacement = QPointF(0.0, 0.0));
    PairRectF(
        const QRect& original_rectf,
        const double& scale_factor,
        const QPointF& center_displacement = QPointF(0.0, 0.0));
    
    // modifier
    
    // modify original QRectF
    // scaled QRectF will change with it 
    void setOriginal(const QRectF& ori);
    void setOriX(const double& x);
    void setOriY(const double& y);
    void setOriWidth(const double& w);
    void setOriHeight(const double& h);
    
    // modify scaled QRectF
    // original QRectF will change with it 
    void setScaled(const QRectF& scal);
    void setScalX(const double& x);
    void setScalY(const double& y);
    void setScalWidth(const double& w);
    void setScalHeight(const double& h);
    
    // change scale factor, scaled QRectF will change
    void setScaleFactor(const double& scale_factor);
    
    // change displacement, scaled QRectF will change
    void setDisplacement(const QPointF& displacement);
    
    // rotate both Original & Scaled QRectF 90 deg
    // switch status between landscape & portrait
    void rotate();
    
    // getter
    
    // original
    const QRectF Original() const;
    const double OriX() const;
    const double OriY() const;
    const double OriWidth() const;
    const double OriHeight() const;
    // scaled
    const QRectF Scaled() const;
    const double ScalX() const;
    const double ScalY() const;
    const double ScalWidth() const;
    const double ScalHeight() const;
    // scale factor
    const double ScaleFactor() const;
    // displacement
    const QPointF Displacement() const;
    
    // Other status
    const bool isNull() const;
    
private:
    QRectF m_Original;
    QRectF m_Scaled;
    /* 
        scale factor 
            = scaled.width() / original.width() 
            = scaled.height() / original.height() 
        scaled.width() = scaled factor * original.width()
        scaled.height() = scaled factor * original.height()
        original.width() = scaled.width() / scaled factor
        original.height() = scaled.height() / scaled factor
    */
    double m_ScaleFactor;
    /*
        Displacement of same point on original & scaled
        Displacement
            = original.point() - scaled.point()
        
        scaled.point() = 
            original.point() - Displacement
        
        original.point() = 
            scaled.point() + Displacement
    */
    QPointF m_Displacement;
    /*
        Formulas with both Scale factor & Displacement
        
        (Scaled_X, Scaled_Y) = 
            ScaleFactor *
            (Original_X, Original_Y) + (Displacement_X, Displacement_Y)
        
        (Original_X, Original_Y) =
            [(Scaled_X, Scaled_Y) - (Displacement_X, Displacement_Y)] /
            ScaleFactor
        
    */
};


// Cursor position relative to
// cropping rect
class CursorPosition
{
public:
    // default constructor
    CursorPosition();
    // parametric constructor
    CursorPosition(
        const QRectF& cropping_rect,
        const QPointF& cursor,
        const double& define_near
    );
    
    // update functions
    void updateCroppingRect(const QRectF& new_cropping_rect);
    void updateCursor(const QPointF& new_cursor);
    
    // getters
    PositionType getPosition();
    const PositionType getPosition() const;
    
private: // helper functions
    // calculate position relativity 
    // of CroppingRect and Cursor
    // and then update m_CursorPositionType
    void calc_relativity();
    
private:
    // define how many pixels are near
    // default to be 10 pixels
    double m_NearDefinition; 
    QRectF m_CroppingRect;
    QPointF m_Cursor;
    PositionType m_CursorPositionType;
};


// ImageEditor Widget
class ImageEditor : public QWidget
{
    Q_OBJECT

public:
    // default constructor
    ImageEditor(QWidget* parent = nullptr);
    
    // basic widget proporties setting
    
    // set/reset widget image
    void setImage(const QString& img_path);
    
    // get image proporties
    const QString getSrcImagePath() const;
    // return image scale factor
    // widget will display an image
    // scaled by this factor
    const double getImageScaleFactor() const;
    const QImage getImage() const;
    // Width Height Ratio = Width / Height
    const double getCroppingWHRatio() const;
    const bool isFixWHRatio() const;
    // get cropping rect & painting rect
    const PairRectF getImageRect() const;
    
    // setting WHRatio of cropper
    void setWHRatio(const QSizeF& width_height);
    void setFixWHRatio2Screen(const bool& flag);
    
public slots: // editing functions
    // crop current image
    void crop();
    // Image rotation
    void rotateL90();
    void rotateR90();
    void mirrorHori();
    void mirrorVrti();
    // rotate only cropping rect, change 
    // m_ImageRect status between Landscape & Portrait
    void rotateCroppingRect();
    // one operation forward / backward
    void undo();
    // saving file
    void save();
    void saveAs(const QString& new_filepath);
    // reset to original image file
    // this function will reload original source image file
    void reset();
    
    // moving cropping rect base on m_CurrImage pixels
    // pix_difference is moving delta (x, y) of this move
    void moveCroppingRectF(const QPointF& pix_difference);
    void moveCroppingRect(const QPoint& pix_difference);
    void resetCroppingRect(); // move to center of widget & reset its size
    
protected: // event handling functions
    virtual void paintEvent(QPaintEvent* event);
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);
    
private: // helper functions
    void update_PaintingImage();
    void update_Displacement();
    void reset_ImageRectF(const QRect& original_img_rect);
    // update ScaleFactor & return new scale factor
    const double update_ScaleFactor(const QRect& new_scale_factor) const;
    const double update_ScaleFactorF(const QRectF& new_scale_factor) const;
    
    void update_CropWHRatio();
    
    QImageReader* setImageReader(const QString& filename);
    
    // update cursor icon base on cursor position
    void update_CursorIcon();
    
    // handle Cropping Rect change during mouse move event
    void mouseMove_WithCustomCropWHRatio(
        const PositionType& curr_pos,
        const double& dx,
        const double& dy,
        const QPointF& topLeft,
        const QPointF& bottomRight,
        QRectF& new_cropping_rect);
    // handle Cropping Rect change during mouse move event
    void mouseMove_WithFixedCropWHRatio(
        const PositionType& curr_pos,
        const double& dx,
        const double& dy,
        const QPointF& topLeft,
        const QPointF& bottomRight,
        QRectF& new_cropping_rect);
    
private:
    // image proporties
    QString m_SrcImagePath;
    QImageReader *p_ImageReader;
    QSize m_ImageScale;
    QImage m_CurrImage;
    QImage m_PrevImage;
    // image for painting
    QImage m_ScaledImage;
    // width height ratio = width / height
    double m_CropWHRatio;
    // this flag will make m_ImageRect have fixed WHRatio
    bool m_IsFixWHRatio;
    
    // cropping rect proporties
    // cropping rect & painting rect
    PairRectF m_ImageRect;
    // buffer: store cursor position from last tick
    QPointF m_LastCursorPos;
    bool m_IsMousePressingDown;
    CursorPosition m_CursorPosition;
    // lock position type during pressed mouse move
    bool m_IsPositionTypeLocked;
    // store PositionType when cursor locked
    PositionType m_LastPositionType;
    
    // other widget proportires
    QColor m_CroppingRectColor;
    QColor m_WidgetBackgroundColor;
};
