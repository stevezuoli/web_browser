#include "ui/DKPushButton.h"    
#include <QPainter>
#include "common/WindowsMetrics.h"
#include "common/debug.h"
#include "Device/device.h"
#include <QDebug>
#include "System/inc/system_manager.h"

namespace ui
{
using namespace windowsmetrics;
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
QPushButton:focus                     \
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
    , shape_type_(ST_Normal)
{
    InitDKProperty();
}

DKPushButton::DKPushButton(const QString& text, QWidget* parent)
    : QPushButton(parent)
    , shape_type_(ST_Normal)
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

void DKPushButton::setFontSize(int fontsize)
{
    setStyleSheet(PUSH_BUTTON_STYLE.arg(fontsize));
}

void DKPushButton::setBackGroundImagePaths(const QString& focusInPath, const QString& focusOutPath)
{
    if (!focusInPath.isEmpty() || !focusOutPath.isEmpty())
    {
        shape_type_ = ST_Image;
    }
    m_focusInBackgroundImage.load(focusInPath);
    m_focusOutBackgroundImage.load(focusOutPath);
}

void DKPushButton::setDisableBackGroundImagePath(const QString& disablePath)
{
    if (!disablePath.isEmpty())
    {
        shape_type_ = ST_Image;
    }

    m_disableBackgroundImage.load(disablePath);
}

void DKPushButton::paintEvent(QPaintEvent* e)
{
    if (shape_type_ == ST_Image)
    {
        QPainter painter(this);
        QFont btnFont = font();
        btnFont.setBold(true);
        btnFont.setPixelSize(ui::windowsmetrics::GetWindowFontSize(ui::windowsmetrics::DKPushButtonIndex));
        setFont(btnFont);
        QImage drawingImage;
        if (isEnabled())
        {
            if (pressed_)
            {
                drawingImage = m_focusInBackgroundImage.isNull() ? drawingImage : m_focusInBackgroundImage;
                painter.setPen(Qt::white);
            }
            else
            {
                drawingImage = m_focusOutBackgroundImage.isNull() ? drawingImage : m_focusOutBackgroundImage;
                painter.setPen(Qt::black);
            }
        }
        else
        {
            drawingImage = m_disableBackgroundImage.isNull() ? drawingImage : m_disableBackgroundImage;
            painter.setPen(Qt::gray);
        }
        if (!drawingImage.isNull())
        {
            QRect targetRect = drawingImage.rect();
            targetRect.moveCenter(rect().center());
            painter.drawImage(targetRect, drawingImage);
        }
        painter.drawText(rect(), Qt::AlignCenter, text());
    }
    else if (shape_type_ == ST_Shadow)
    {
        QPainter painter(this);
        QFont btnFont = font();
        btnFont.setBold(true);
        btnFont.setPixelSize(ui::windowsmetrics::GetWindowFontSize(ui::windowsmetrics::DKPushButtonIndex));
        setFont(btnFont);
        qreal radius = 10.0;
        QRect bgRect = rect().adjusted(0, 0, -GetWindowMetrics(DKButtonBorderPixelIndex), -GetWindowMetrics(DKButtonBorderPixelIndex));
        //QRect bgRect = rect().adjusted(0, 0, -GetWindowMetrics(DKButtonBorderHorPixelIndex), -GetWindowMetrics(DKButtonBorderBottomPixelIndex));
        if (pressed_)
        {
            painter.setBrush(Qt::black);
            painter.drawRoundedRect(bgRect, radius, radius);
            painter.setPen(Qt::white);
            painter.drawText(rect(), Qt::AlignCenter, text());
        }
        else
        {
            painter.fillRect(rect(), Qt::white);
            painter.setPen(Qt::black);
            QPen btnPen = painter.pen();
            btnPen.setWidth(GetWindowMetrics(DKButtonBorderPixelIndex));
            painter.setPen(btnPen);
            painter.drawRoundedRect(bgRect, radius, radius);
            painter.drawText(rect(), Qt::AlignCenter, text());
            //painter.setBrush(Qt::black);
            //painter.drawRect(leftRect);
            //painter.drawRect(rightRect);
            //painter.drawRect(bottomRect);
        }
    }
    else if (shape_type_ == ST_Normal)
    {
        //DebugWB("%x, %d, %d, %d", this, hasFocus(), isChecked(), isDown());
        QPushButton::paintEvent(e);
    }
}

void DKPushButton::mousePressEvent(QMouseEvent* e)
{
    //DebugWB("%x", this);
    pressed_ = true;
    QPushButton::mousePressEvent(e);
}

void DKPushButton::mouseReleaseEvent(QMouseEvent* e)
{
    //DebugWB("%x", this);
    pressed_ = false;
    QPushButton::mouseReleaseEvent(e);
}

void DKPushButton::setShapeType(ShapeType shapeType)
{
    shape_type_ = shapeType;
    if (shape_type_ != ST_Normal)
    {
        setStyleSheet("");
    }
}
}//ui
