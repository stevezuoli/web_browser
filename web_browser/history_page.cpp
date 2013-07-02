#include "web_browser/history_page.h"
#include "web_browser/history_list_item.h"
#include "web_browser/view.h"
#include "common/WindowsMetrics.h"
#include "common/debug.h"
#include "ui/DKPushButton.h"

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

    connect(&history_list_, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(onItemClicked(QListWidgetItem*)));
    DKPushButton* btn = bottom_buttons_.addButton(tr("Clear history data"));
    connect(btn, SIGNAL(clicked(bool)), this, SLOT(clearHistoryData(bool)));
    btn = bottom_buttons_.addButton(tr("Close"));
    connect(btn, SIGNAL(clicked(bool)), this, SLOT(onCloseButtonClicked(bool)));
    InitLayout();
}

void HistoryPage::paintEvent(QPaintEvent* event)
{
    QWidget::paintEvent(event);
}

void HistoryPage::InitLayout()
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
        InitItems();
    }
    QWidget::setVisible(visible);
}

void HistoryPage::InitItems()
{
    if (!view_)
    {
        return;
    }
    history_list_.clear();
    QVariantList varList = view_->GetSiteList();
    qSort(varList.begin(), varList.end(), WebThumbnail::dateTimeGreaterThan);
    QString lastDate;// = ((const WebThumbnail&)(varList.at(0).toMap())).accessDate();
    int w = GetWindowMetrics(HistoryItemLabelWidthIndex);
    //int w = GetWindowMetrics(UIScreenWidthIndex) - (GetWindowMetrics(UIHorizonMarginIndex) << 1);
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
    emit historyPageQuit("");
}
}
