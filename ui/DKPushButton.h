#ifndef __UI_DKPUSHBUTTON_H__
#define __UI_DKPUSHBUTTON_H__

#include <QAbstractButton>
namespace ui
{
class DKPushButton : public QAbstractButton
{
    Q_OBJECT

public:
    DKPushButton(QWidget* parent = 0);
    DKPushButton(const QString& text, QWidget* parent = 0);
    DKPushButton(const QString& iconPath, const QString& text, QWidget* parent = 0);

    void setBackGroundImagePaths(const QString& focusInPath, const QString& focusOutPath);
    void setPressed(bool press)
    {
        pressed_ = press;
        update();
    }
protected:
    virtual void paintEvent(QPaintEvent*);
    virtual void mousePressEvent(QMouseEvent* e);
    virtual void mouseReleaseEvent(QMouseEvent* e);

private:
    void setDKStyleSheet();
    void InitDKProperty();
    QPixmap m_focusInBackgroundImage;
    QPixmap m_focusOutBackgroundImage;
    bool pressed_;
};//DKPushButton

}
#endif//__UI_DKPUSHBUTTON_H__
