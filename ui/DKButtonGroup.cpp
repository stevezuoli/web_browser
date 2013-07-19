#include "ui/DKButtonGroup.h"
#include "ui/DKPushButton.h"
#include "common/WindowsMetrics.h"
#include "common/debug.h"
#include <QKeyEvent>

#include <QPainter>

namespace ui
{
using namespace windowsmetrics;

DKButtonGroup::DKButtonGroup(QWidget* parent)
    : QWidget(parent)
    , topline_pixel(1)
{
    setFocusPolicy(Qt::StrongFocus);
    layout_.setSpacing(GetWindowMetrics(MessageBoxButtonSpaceIndex));
    setLayout(&layout_);
}

DKPushButton* DKButtonGroup::addButton(const QString& text)
{
    DKPushButton* btn = new DKPushButton(text, this);
    buttons_.push_back(btn);
    btn->installEventFilter(this);
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

//void DKButtonGroup::keyReleaseEvent(QKeyEvent* event)
//{
    //DebugWB("");
    //switch (event->key())
    //{
        //case Qt::Key_Left:
        //case Qt::Key_Right:
            //event->accept();
            //return;
    //}
    //event->ignore();
    //QWidget::keyReleaseEvent(event);
//}

//void DKButtonGroup::keyPressEvent(QKeyEvent* event)
//{
    //DebugWB("");
    //switch (event->key())
    //{
        //case Qt::Key_Left:
        //case Qt::Key_Right:
            //focusNextPrevButton(Qt::Key_Right);
            //event->accept();
            //return;
    //}
    //event->ignore();
    //QWidget::keyPressEvent(event);
//}

//void DKButtonGroup::focusNextPrevButton(bool next)
//{
    //int focusIndex = -1;
    //for (int i = 0; i < buttons_.size(); ++i)
    //{
        //if (buttons_[i]->hasFocus())
        //{
            //focusIndex = i;
            //break;
        //}
    //}

    //next ? ++focusIndex : --focusIndex;
    //focusIndex = focusIndex >= 0 ? focusIndex : buttons_.size() - 1;
    //focusIndex = focusIndex < buttons_.size() ? focusIndex : 0;
    //buttons_[focusIndex]->setFocus();
//}

void DKButtonGroup::setFocusOnButtonIndex(int index)
{
    if (index >= 0 && index < buttons_.size())
    {
        buttons_[index]->setFocus();
    }
}

int DKButtonGroup::getFocusIndex()
{
    for (unsigned int i = 0; i < buttons_.size(); ++i)
    {
        //DebugWB("i = %d, focus: %d", i, buttons_[i]->hasFocus());
        if (buttons_[i]->hasFocus())
            return i;
    }

    return -1;
}

bool DKButtonGroup::eventFilter(QObject* watched, QEvent* event)
{
    unsigned int btnsCount = buttons_.size();
    if (btnsCount > 1 && event->type() == QEvent::KeyPress)
    {
        //DebugWB("%d", btnsCount);
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        if (watched == buttons_[0] && keyEvent->key() == Qt::Key_Left)
        {
            //DebugWB("first and key left.");
            buttons_[btnsCount - 1]->setFocus();
            return true;
        }
        else if (watched == buttons_[btnsCount - 1] && keyEvent->key() == Qt::Key_Right)
        {
            //DebugWB("last and key right.");
            buttons_[0]->setFocus();
            return true;
        }
    }

    return QWidget::eventFilter(watched, event);
}
}//ui
