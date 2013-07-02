#include "ui/DKPushButton.h"    
#include <QPainter>
#include "common/WindowsMetrics.h"
#include "Device/device.h"
#include <QDebug>

namespace ui
{
static const QString PUSH_BUTTON_STYLE = "                          \
QPushButton                             \
{                                       \
    font: %1px;                         \
    border-width: 0px;                  \
    border-color: transparent;          \
    background-color: transparent;                       \
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
    background-color: black;            \
}                                       \
QPushButton:disabled                    \
{                                       \
    padding-left: 0px;                  \
    padding-top: 0px;                   \
    border-color: dark;                 \
    color: dark;                        \
    background: white;                  \
}";

DKPushButton::DKPushButton(QWidget* parent)
    : QPushButton(parent)
    , pressed_(false)
{
    InitDKProperty();
}

DKPushButton::DKPushButton(const QString& text, QWidget* parent)
    : QPushButton(parent)
{
    setText(text);
    InitDKProperty();
}

void DKPushButton::setDKStyleSheet()
{
    setStyleSheet(PUSH_BUTTON_STYLE.
            arg(ui::windowsmetrics::GetWindowFontSize(ui::windowsmetrics::DKPushButtonIndex)));
}

void DKPushButton::InitDKProperty()
{
    setDKStyleSheet();
}

void DKPushButton::setBackGroundImagePaths(const QString& focusInPath, const QString& focusOutPath)
{
    m_focusInBackgroundImage.load(focusInPath);
    m_focusOutBackgroundImage.load(focusOutPath);
}

void DKPushButton::paintEvent(QPaintEvent* e)
{
    if (!m_focusInBackgroundImage.isNull() && !m_focusOutBackgroundImage.isNull())
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
    else
    {
        QPushButton::paintEvent(e);
    }
}

void DKPushButton::mousePressEvent(QMouseEvent* e)
{
    pressed_ = true;
    QPushButton::mousePressEvent(e);
}

void DKPushButton::mouseReleaseEvent(QMouseEvent* e)
{
    pressed_ = false;
    QPushButton::mouseReleaseEvent(e);
}
}//ui
