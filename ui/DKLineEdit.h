#ifndef __UI_DKLINEEDIT_H__
#define __UI_DKLINEEDIT_H__

#include <QLineEdit>

namespace ui
{
class DKLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    DKLineEdit(QWidget* parent = 0);
    DKLineEdit(const QString& contents, QWidget* parent = 0);

protected:
    virtual void focusInEvent(QFocusEvent* e);
    virtual void focusOutEvent(QFocusEvent* e);
    virtual void mousePressEvent(QMouseEvent* event);

signals:
    void focusSignal(bool);

private:
    void setDKStyleSheet();
    void InitDKProperty();
    bool selectall_in_mouse_press_;
};//DKLineEdit
}
#endif//__UI_DKLINEEDIT_H__
