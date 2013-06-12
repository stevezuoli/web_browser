#include "ui/DKToolBar.h"
#include <QPainter>
#include <QStyleOption>

namespace ui
{
const QString TOOL_BAR_STYLE = "\
QToolBar                        \
{                               \
    background: transparent;    \
    spacing: 3px;               \
}";                             

DKToolBar::DKToolBar(const QString& title, QWidget* parent)
    : QToolBar(title, parent)
{
    InitDKProperty();
}

DKToolBar::DKToolBar(QWidget* parent)
    : QToolBar(parent)
{
    InitDKProperty();
}
  
void DKToolBar::InitDKProperty()
{
    //setMovable(false);
    setDKStyleSheet();
}

void DKToolBar::setDKStyleSheet()
{
    //setStyleSheet(TOOL_BAR_STYLE);
}

//void DKToolBar::paintEvent(QPaintEvent* e)
//{
//    QPainter p(this);
//    p.fillRect(rect(), QBrush(QColor(255,255,255)));
//}
}//ui
