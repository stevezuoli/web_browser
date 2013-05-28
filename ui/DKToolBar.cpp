#include "ui/DKToolBar.h"

namespace ui
{
const QString TOOL_BAR_STYLE = "\
QToolBar                        \
{                               \
    background: white;          \
    spacing: 3px;               \
}";                             \

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
    setMovable(false);
    setDKStyleSheet();
}

void DKToolBar::setDKStyleSheet()
{
    setStyleSheet(TOOL_BAR_STYLE);
}
}//ui
