#ifndef __UI_DKLISTWIDGET_H__
#define __UI_DKLISTWIDGET_H__

#include <QListWidget>
namespace ui
{
class DKListWidget : public QListWidget
{
public:
    DKListWidget(QWidget* parent = 0);

protected:
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);

private:
    QPoint position_;
};//DKListWidget
}//ui

#endif//__UI_DKLISTWIDGET_H__
