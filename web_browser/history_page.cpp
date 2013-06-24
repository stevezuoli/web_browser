#include "web_browser/history_page.h"
#include "web_browser/history_list_item.h"
#include "web_browser/view.h"
#include "common/WindowsMetrics.h"
#include "common/debug.h"

using namespace ui::windowsmetrics;
namespace webbrowser
{
HistoryPage::HistoryPage(BrowserView* view, QWidget* parent)
    : QWidget(parent)
    , history_list_(this)
    , clear_button_(tr("Clear history data"), this)
    , close_button_(tr("Close"), this)
    , view_(view)
{
    history_list_.setFrameShape(QFrame::NoFrame);
    //history_list_.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //history_list_.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    history_list_.verticalScrollBar()->setStyleSheet(
            "QScrollBar:vertical{ background: white; min-width: 20px; border: 2px solid black}"
            "QScrollBar::handle:vertical{ background: black; min-height: 100px}"
            "QScrollBar::sub-page:vertical{background: white}"
            "QScrollBar::add-page:vertical{background: white}"
            );

    connect(&clear_button_, SIGNAL(clicked(bool)), this, SLOT(clearHistoryData(bool)));
    connect(&close_button_, SIGNAL(clicked(bool)), this, SIGNAL(onHistoryPageQuit(bool)));
    InitLayout();
}

void HistoryPage::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    int lineTop = clear_button_.pos().y();
    int linePixel = GetWindowMetrics(NavigationBarLinePixelIndex);
    painter.setBrush(Qt::black);
    painter.drawRect(0, lineTop - linePixel, GetWindowMetrics(UIScreenWidthIndex), linePixel);
    int border = 5;
    painter.drawRect(width() >> 1, lineTop + border, 1, clear_button_.height() - (border << 1));
    QWidget::paintEvent(event);
}

void HistoryPage::InitLayout()
{
    main_layout_.addWidget(&history_list_);
    main_layout_.addLayout(&bottom_layout_);
    bottom_layout_.addWidget(&clear_button_);
    bottom_layout_.addSpacing(GetWindowMetrics(HistoryButtonSpaceIndex));
    bottom_layout_.addWidget(&close_button_);
    clear_button_.setFixedHeight(GetWindowMetrics(HistoryButtonHeightIndex));
    close_button_.setFixedHeight(GetWindowMetrics(HistoryButtonHeightIndex));

    setLayout(&main_layout_);
}

void HistoryPage::setVisible(bool visible)
{
    if (view_)
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
    QString lastDate;// = ((const WebThumbnail&)(varList.at(0).toMap())).accessDate();
    for(int i = 0; i < varList.size(); ++i)
    {
        QString thisDate = ((const WebThumbnail&)(varList.at(i).toMap())).accessDate();
        if (lastDate.compare(thisDate) != 0)
        {
            QListWidgetItem* item = new QListWidgetItem;
            item->setSizeHint(QSize(width() - 10, GetWindowMetrics(HistoryItemHeightIndex)));
            HistoryListItem* historyListItem = new HistoryListItem(thisDate);
            history_list_.addItem(item);
            history_list_.setItemWidget(item, historyListItem);
            lastDate = thisDate;
        }

        QListWidgetItem* item = new QListWidgetItem;
        item->setSizeHint(QSize(width() - 10, GetWindowMetrics(HistoryItemHeightIndex)));
        HistoryListItem* historyListItem = new HistoryListItem(varList.at(i).toMap());
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
}
