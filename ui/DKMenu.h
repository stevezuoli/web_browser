#ifndef __UI_DKMENU_H_
#define __UI_DKMENU_H_

#include <QtGui/QMenu>

namespace ui
{
class DKMenu : public QMenu
{
public:
    DKMenu(const QString& title, QWidget* parent = 0);
    DKMenu(QWidget* parent);

private:
    void setDKStyleSheet();
    void InitDKProperty();
};
}//ui
#endif//__UI_DKMENU_H_
