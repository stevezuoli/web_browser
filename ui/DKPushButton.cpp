#include "ui/DKPushButton.h"    
#include <QPainter>

namespace ui
{
static const QString PUSH_BUTTON_STYLE = "                          \
QPushButton                             \
{                                       \
    background: transparent;            \
    border: 0px                         \
    border-width: 0px;                  \
    border-color: transparent;          \
    border-style: solid;                \
    color: black;                       \
    padding: 0px;                       \
}                                       \
QPushButton:pressed                     \
{                                       \
    padding-left: 0px;                  \
    padding-top: 0px;                   \
    color: white;                       \
    border-color: white;                \
    background-color: black;            \
}                                       \
QPushButton:checked                     \
{                                       \
    padding-left: 0px;                  \
    padding-top: 0px;                   \
    color: white;                       \
    border-color: white;                \
    background-color: black;            \
}                                       \
QPushButton:focus                       \
{                                       \
    padding-left: 0px;                  \
    padding-top: 0px;                   \
    border: 0px                         \
    color: black;                       \
    border-width: 0px;                  \
    border-color: white;                \
    background-color: white;            \
}                                       \
QPushButton:disabled                    \
{                                       \
    padding-left: 0px;                  \
    padding-top: 0px;                   \
    border-color: dark;                 \
    color: dark;                        \
    background-color: white;            \
}";

DKPushButton::DKPushButton(QWidget* parent)
    : QPushButton(parent)
{
    InitDKProperty();
}
DKPushButton::DKPushButton(const QString& text, QWidget* parent)
    : QPushButton(text, parent)
{
    InitDKProperty();
}

DKPushButton::DKPushButton(const QIcon& icon, const QString& text, QWidget* parent)
    : QPushButton(icon, text, parent)
{
    InitDKProperty();
}

void DKPushButton::setDKStyleSheet()
{
    setStyleSheet(PUSH_BUTTON_STYLE);
}

void DKPushButton::InitDKProperty()
{
    setFlat(true);
}

void DKPushButton::paintEvent(QPaintEvent* e)
{
    if (isDown())
    {
        if (!m_focusInBackgroudImagePath.isEmpty())
        {
            QPainter painter(this);
            painter.drawPixmap(rect(), QPixmap(m_focusInBackgroudImagePath));
        }
    }
    else
    {
        if (!m_focusInBackgroudImagePath.isEmpty())
        {
            QPainter painter(this);
            painter.drawPixmap(rect(), QPixmap(m_focusInBackgroudImagePath));
        }
    }

    return QPushButton::paintEvent(e);
}
}//ui
