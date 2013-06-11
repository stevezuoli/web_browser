#include "ui/DKPushButton.h"    
#include <QPainter>
#include "common/WindowsMetrics.h"

namespace ui
{
static const QString PUSH_BUTTON_STYLE = "                          \
QAbstractButton                             \
{                                       \
    background: transparent;            \
    border: 0px                         \
    border-width: 0px;                  \
    border-color: transparent;          \
    border-style: solid;                \
    color: black;                       \
    padding: 0px;                       \
}                                       \
QAbstractButton:pressed                     \
{                                       \
    padding-left: 0px;                  \
    padding-top: 0px;                   \
    color: white;                       \
    border-color: white;                \
    background-color: black;            \
}                                       \
QAbstractButton:checked                     \
{                                       \
    padding-left: 0px;                  \
    padding-top: 0px;                   \
    color: white;                       \
    border-color: white;                \
    background-color: black;            \
}                                       \
QAbstractButton:focus                       \
{                                       \
    padding-left: 0px;                  \
    padding-top: 0px;                   \
    border: 0px                         \
    color: black;                       \
    border-width: 0px;                  \
    border-color: white;                \
    background-color: white;            \
}                                       \
QAbstractButton:disabled                    \
{                                       \
    padding-left: 0px;                  \
    padding-top: 0px;                   \
    border-color: dark;                 \
    color: dark;                        \
    background-color: white;            \
}";

DKPushButton::DKPushButton(QWidget* parent)
    : QAbstractButton(parent)
    , pressed_(false)
{
    InitDKProperty();
}

DKPushButton::DKPushButton(const QString& text, QWidget* parent)
    : QAbstractButton(parent)
{
    setText(text);
    InitDKProperty();
}

DKPushButton::DKPushButton(const QString& path, const QString& text, QWidget* parent)
    : QAbstractButton(parent)
{
    setText(text);
    InitDKProperty();
    setBackGroundImagePaths(path, "");
}

void DKPushButton::setDKStyleSheet()
{
    setStyleSheet(PUSH_BUTTON_STYLE);
}

void DKPushButton::InitDKProperty()
{
}

void DKPushButton::setBackGroundImagePaths(const QString& focusInPath, const QString& focusOutPath)
{
    m_focusInBackgroundImage.load(focusInPath);
    m_focusOutBackgroundImage.load(focusOutPath);
}

void DKPushButton::paintEvent(QPaintEvent* e)
{
    QPainter painter(this);
    QFont btnFont = font();
    btnFont.setBold(true);
    btnFont.setPixelSize(ui::windowsmetrics::GetWindowFontSize(ui::windowsmetrics::DKPushButtonIndex));
    setFont(btnFont);
    if (pressed_)
    {
        if (!m_focusInBackgroundImage.isNull())
        {
            painter.drawPixmap(rect(), m_focusInBackgroundImage);
        }
        painter.setPen(Qt::white);
        painter.drawText(rect(), Qt::AlignCenter, text());
    }
    else
    {
        if (!m_focusOutBackgroundImage.isNull())
        {
            painter.drawPixmap(rect(), m_focusOutBackgroundImage);
        }
        painter.setPen(Qt::black);
        painter.drawText(rect(), Qt::AlignCenter, text());
    }

}

void DKPushButton::mousePressEvent(QMouseEvent* e)
{
    pressed_ = true;
    QAbstractButton::mousePressEvent(e);
}

void DKPushButton::mouseReleaseEvent(QMouseEvent* e)
{
    pressed_ = false;
    QAbstractButton::mouseReleaseEvent(e);
}
}//ui
