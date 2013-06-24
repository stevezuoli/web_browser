#include "ui/DKMenu.h"
#include <QStyleOption>
#include "common/WindowsMetrics.h"

namespace ui
{
using namespace windowsmetrics;
//different styles should be used on kt&kp
const QString MENU_STYLE = "\
QMenu                        \
{                               \
    background-color: white;    \
    spacing: 3px;               \
    border: 1px solid black; \
    font: %1px;             \
}                           \
QMenu::item                       \
{                           \
    padding: %2px;          \
    background-color: transparent;\
}                           \
QMenu::item::selected                       \
{                           \
    background-color: black;\
}                           \
QMenu::separator            \
{                           \
    height: 2px;            \
    background:black;       \
    margin-left:%3px;         \
    margin-right:%4px;          \
}";                             

DKMenu::DKMenu(const QString& title, QWidget* parent)
    : QMenu(title, parent)
{
    InitDKProperty();
}

DKMenu::DKMenu(QWidget* parent)
    : QMenu(parent)
{
    InitDKProperty();
}
  
void DKMenu::InitDKProperty()
{
    setDKStyleSheet();
}

void DKMenu::setDKStyleSheet()
{
    setStyleSheet(MENU_STYLE.
            arg(GetWindowFontSize(DKMenuIndex)).
            arg(GetWindowMetrics(MenuItemPaddingIndex)).
            arg(GetWindowMetrics(MenuSeparatorPaddingIndex)).
            arg(GetWindowMetrics(MenuSeparatorPaddingIndex)));
}
}//ui
