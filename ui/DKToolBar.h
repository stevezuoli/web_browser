#ifndef __UI_DKTOOLBAR_H_
#define __UI_DKTOOLBAR_H_

#include <QtGui/QToolBar>

namespace ui
{
class DKToolBar : public QToolBar
{
public:
    DKToolBar(const QString& title, QWidget* parent = 0);
    DKToolBar(QWidget* parent);

protected:
    //virtual void paintEvent(QPaintEvent* e);
private:
    void setDKStyleSheet();
    void InitDKProperty();
};
}//ui
#endif//__UI_DKTOOLBAR_H_
