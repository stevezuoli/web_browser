#ifndef __UI_DKPUSHBUTTON_H__
#define __UI_DKPUSHBUTTON_H__

#include <QPushButton>
namespace ui
{
class DKPushButton : public QPushButton
{
    Q_OBJECT

public:
    enum ShapeType
    {
        ST_Shadow,
        ST_Image,
        ST_Normal,
    };

    DKPushButton(QWidget* parent = 0);
    DKPushButton(const QString& text, QWidget* parent = 0);

    void setFontSize(int fontsize);
    void setBackGroundImagePaths(const QString& focusInPath, const QString& focusOutPath);
    void setDisableBackGroundImagePath(const QString& disablePath);
    void setPressed(bool press)
    {
        pressed_ = press;
        update();
    }
    void setShapeType(ShapeType shapeType);
protected:
    virtual void paintEvent(QPaintEvent*);
    virtual void mousePressEvent(QMouseEvent* e);
    virtual void mouseReleaseEvent(QMouseEvent* e);

private:
    void setDKStyleSheet();
    void InitDKProperty();
    QImage m_focusInBackgroundImage;
    QImage m_focusOutBackgroundImage;
    QImage m_disableBackgroundImage;
    bool pressed_;
    ShapeType shape_type_;
};//DKPushButton

}
#endif//__UI_DKPUSHBUTTON_H__
