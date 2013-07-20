#include "ui/DKButtonGroup.h"
#include "ui/DKPushButton.h"
#include "common/WindowsMetrics.h"
#include <QDebug>

#include <QPainter>

namespace ui
{
using namespace windowsmetrics;

DKButtonGroup::DKButtonGroup(QWidget* parent)
    : QWidget(parent)
    , topline_pixel(1)
{
    layout_.setSpacing(GetWindowMetrics(MessageBoxButtonSpaceIndex));
    setLayout(&layout_);
}

DKPushButton* DKButtonGroup::addButton(const QString& text)
{
    DKPushButton* btn = new DKPushButton(text, this);
    buttons_.push_back(btn);
    QSizePolicy policy = sizePolicy();
    policy.setHorizontalPolicy(QSizePolicy::Expanding);
    btn->setSizePolicy(policy);
    layout_.addWidget(btn);
    return btn;
}

void DKButtonGroup::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setBrush(Qt::black);
    painter.drawRect(0, 0, width(), topline_pixel);
    for (int i = 1; i < buttons_.size(); ++i)
    {
        DKPushButton* btn = buttons_[i];
        int x = btn->x() - (layout_.spacing() >> 1);
        painter.drawLine(x, btn->y(), x, btn->y() + btn->height());
    }
    QWidget::paintEvent(event);
}
}//ui
