#include "web_browser/history_page.h"
#include "web_browser/history_list_item.h"
#include "web_browser/view.h"
#include "common/WindowsMetrics.h"
#include "common/debug.h"
#include "ui/DKPushButton.h"
#include <algorithm>

using namespace ui::windowsmetrics;
namespace webbrowser
{
HistoryPage::HistoryPage(BrowserView* view, QWidget* parent)
    : QWidget(parent)
    , history_list_(this)
    , bottom_buttons_(this)
    , view_(view)
{
    history_list_.setStyleSheet(QString("QListWidget{margin-left: %1px; margin-right: 15px}").arg(GetWindowMetrics(UIHorizonMarginIndex)));
    history_list_.setFrameShape(QFrame::NoFrame);
    history_list_.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    history_list_.verticalScrollBar()->setStyleSheet(
            "QScrollBar:vertical{ background: white; min-width: 20px; border: 1px solid black;}"
            "QScrollBar::handle:vertical{ background: black; min-height: 100px}"
            "QScrollBar::sub-page:vertical{background: white}"
            "QScrollBar::add-page:vertical{background: white}"
            );
    history_list_.installEventFilter(this);

    connect(&history_list_, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(onItemClicked(QListWidgetItem*)));
    DKPushButton* btn = bottom_buttons_.addButton(tr("Clear history data"));
    connect(btn, SIGNAL(clicked(bool)), this, SLOT(clearHistoryData(bool)));
    bottom_buttons_container.push_back(btn);
    btn = bottom_buttons_.addButton(tr("Close"));
    bottom_buttons_container.push_back(btn);
    connect(btn, SIGNAL(clicked(bool)), this, SLOT(onCloseButtonClicked(bool)));

    foreach(btn, bottom_buttons_container)
    {
        btn->installEventFilter(this);
    }
    initLayout();
}

void HistoryPage::paintEvent(QPaintEvent* event)
{
    //DebugWB("");
    QWidget::paintEvent(event);
}

void HistoryPage::initLayout()
{
    main_layout_.addWidget(&history_list_);
    main_layout_.addWidget(&bottom_buttons_);
    main_layout_.setSpacing(0);
    main_layout_.setContentsMargins(0, 0, 0, 0);
    bottom_buttons_.setFixedHeight(GetWindowMetrics(HistoryButtonHeightIndex));

    setLayout(&main_layout_);
}

void HistoryPage::setVisible(bool visible)
{
    if (view_ && visible)
    {
        initItems();
    }
    QWidget::setVisible(visible);
}

void HistoryPage::initItems()
{
    if (!view_)
    {
        return;
    }
    history_list_.clear();
    QVariantList varList = view_->GetSiteList();
    qSort(varList.begin(), varList.end(), WebThumbnail::dateTimeGreaterThan);
    QString lastDate;
    int w = GetWindowMetrics(HistoryItemLabelWidthIndex);
    for(int i = 0; i < varList.size(); ++i)
    {
        QString thisDate = ((const WebThumbnail&)(varList.at(i).toMap())).accessDate();
        if (lastDate.compare(thisDate) != 0)
        {
            QListWidgetItem* item = new QListWidgetItem;
            item->setSizeHint(QSize(w, GetWindowMetrics(HistoryItemHeightIndex)));
            HistoryListItem* historyListItem = new HistoryListItem(thisDate);
            if (i == 0)
            {
                historyListItem->setLinePixel(0);
            }
            item->setFlags(Qt::NoItemFlags);
            history_list_.addItem(item);
            history_list_.setItemWidget(item, historyListItem);
            lastDate = thisDate;
        }

        QListWidgetItem* item = new QListWidgetItem;
        item->setSizeHint(QSize(w, GetWindowMetrics(HistoryItemHeightIndex)));
        HistoryListItem* historyListItem = new HistoryListItem(varList.at(i).toMap());
        historyListItem->elideTextWithWidth(w);
        history_list_.addItem(item);
        history_list_.setItemWidget(item, historyListItem);
    }
    if (varList.size() > 0)
    {
        history_list_.setFocus();
        history_list_.setCurrentRow(1);
        QWidget* widget = history_list_.itemWidget(history_list_.item(1)); 
        if (widget)
        {
            widget->setFocus();
        }
    }
    else
    {
        bottom_buttons_.setFocusOnButtonIndex(1);
    }
}

void HistoryPage::clearHistoryData(bool)
{
    {
        history_list_.clear();
        if (view_)
        {
            view_->clearHistoryData();
        }
    }
}

void HistoryPage::onItemClicked(QListWidgetItem* item)
{
    HistoryListItem* widget = reinterpret_cast<HistoryListItem*>(history_list_.itemWidget(item));
    if (widget && !widget->isDateItem())
    {
        emit historyPageQuit(widget->getUrl());
    }
}

void HistoryPage::onCloseButtonClicked(bool)
{
    QUrl empty;
    emit historyPageQuit(empty);
}

//void HistoryPage::keyPressEvent(QKeyEvent* event)
//{
    //DebugWB("%d, %d, %d, %d", history_list_.hasFocus(), bottom_buttons_.hasFocus(), hasFocus(), history_list_.currentRow());
    //switch (event->key())
    //{
        //case Qt::Key_Up:
        //case Qt::Key_Down:
            //if (onUpDownKeyPressed(event->key() == Qt::Key_Down))
            //{
                //DebugWB("onUpDownKeyPressed return true");
                //event->accept();
            //}
            //break;
        //case Qt::Key_Select:
            //event->accept();
            //return;
    //}
    //QWidget::keyPressEvent(event);
//}

//void HistoryPage::keyReleaseEvent(QKeyEvent* event)
//{
    //DebugWB("%d, %d, %d, %d", history_list_.hasFocus(), bottom_buttons_.hasFocus(), hasFocus(), history_list_.currentRow());
    //switch (event->key())
    //{
        //case Qt::Key_Up:
        //case Qt::Key_Down:
        //case Qt::Key_Select:
            //DebugWB("KEY_SELECT pressed.");
            //onItemClicked(history_list_.currentItem());
            //event->accept();
            //return;
    //}
    //QWidget::keyReleaseEvent(event);
//}

//bool HistoryPage::onUpDownKeyPressed(bool down)
//{
    //int row = history_list_.currentRow();
    //QWidget* widget = history_list_.itemWidget(history_list_.currentItem()); 
    //DebugWB("%d, %d, %d, %d", history_list_.hasFocus(), bottom_buttons_.hasFocus(), hasFocus(), history_list_.currentRow());
    //DebugWB("%x, %d, %d, %d", widget, widget ? widget->hasFocus() : 0, bottom_buttons_.hasFocus(), bottom_buttons_.getFocusIndex());
    //if (bottom_buttons_.getFocusIndex() != -1)
    //{
        //DebugWB("%d",history_list_.count());
        //if (history_list_.count() > 1)
        //{
            //QListWidgetItem* item = down ? history_list_.item(1) : history_list_.item(history_list_.count() - 1);
            //QWidget* widget = history_list_.itemWidget(item);
            //if (widget)
            //{
                //widget->setFocus();
            //}
        //}
        //return true;
    //}
    //else if (widget && widget->hasFocus())
    //{
        //if ((row == 1 && !down) || ((row == history_list_.count()-1) && down))
        //{
            //DebugWB("set focus button: %d, %d", row, widget);
            //widget->clearFocus();
            //history_list_.setCurrentRow(-1);
            //bottom_buttons_.setFocusOnButtonIndex(1);
            //return true;
        //}
    //}

    //return false;
//}

bool HistoryPage::eventFilter(QObject* watched, QEvent* event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        if (std::find(bottom_buttons_container.begin(), bottom_buttons_container.end(), watched) != bottom_buttons_container.end())
        {
            if (keyEvent->key() == Qt::Key_Up || keyEvent->key() == Qt::Key_Down)
            {
                if (history_list_.count() > 0)
                {
                    history_list_.setFocus();
                    history_list_.setCurrentRow(1);
                    QWidget* widget = history_list_.itemWidget(history_list_.item(1)); 
                    if (widget)
                    {
                        widget->setFocus();
                    }
                }

                return true;
            }
        }
        else if (watched == &history_list_)
        {
            if (keyEvent->key() == Qt::Key_Select)
            {
                onItemClicked(history_list_.currentItem());
                return true;
            }

            int row = history_list_.currentRow();
            QWidget* widget = history_list_.itemWidget(history_list_.currentItem()); 
            if (widget && widget->hasFocus())
            {
                if ((row == 1 && keyEvent->key() == Qt::Key_Up) || ((row == history_list_.count()-1) && keyEvent->key() == Qt::Key_Down))
                {
                    widget->clearFocus();
                    history_list_.setCurrentRow(-1);
                    bottom_buttons_.setFocusOnButtonIndex(1);
                    return true;
                }
            }
        }
    }

    return QWidget::eventFilter(watched, event);
}
}
