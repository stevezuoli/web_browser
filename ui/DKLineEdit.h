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

private:
    void setDKStyleSheet();
    void InitDKProperty();
};//DKLineEdit
}
#endif//__UI_DKLINEEDIT_H__
