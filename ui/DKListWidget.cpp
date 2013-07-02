#include "ui/DKListWidget.h"
#include "common/WindowsMetrics.h"
#include "common/debug.h"

#include <QScrollBar>
#include <QMouseEvent>

namespace ui
{
using namespace windowsmetrics;
DKListWidget::DKListWidget(QWidget* parent)
    : QListWidget(parent)
{
}

void DKListWidget::mousePressEvent(QMouseEvent* event)
{
    position_ = event->pos();
    //QListWidget::mousePressEvent(event);
    event->accept();
}

void DKListWidget::mouseMoveEvent(QMouseEvent* event)
{
    event->accept();
}

void DKListWidget::mouseReleaseEvent(QMouseEvent* event)
{
    if (!position_.isNull())
    {
        QPoint delta = position_ - event->pos();
        if (qAbs(delta.y()) < GetWindowMetrics(ListWidgetSingleStepPixelIndex))
        {
            emit itemClicked(itemAt(event->pos()));
        }
        else
        {
            verticalScrollBar()->setValue(verticalScrollBar()->value() +
                verticalScrollBar()->singleStep() * (double(delta.y())/GetWindowMetrics(ListWidgetSingleStepPixelIndex)));
        }
        //QListWidget::mouseReleaseEvent(event);
        event->accept();
        position_.rx() = position_.ry() = 0;
    }
}
}
