#ifndef __UI_DKMENU_H_
#define __UI_DKMENU_H_

#include <QtGui/QMenu>

namespace ui
{
class DKMenu : public QMenu
{
    Q_OBJECT
public:
    DKMenu(const QString& title, QWidget* parent = 0);
    DKMenu(QWidget* parent);
    virtual void setVisible(bool visible);

Q_SIGNALS:
    void becomeVisible(bool visible);

protected:
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void keyReleaseEvent(QKeyEvent* event);

private:
    void setDKStyleSheet();
    void InitDKProperty();
};
}//ui
#endif//__UI_DKMENU_H_
