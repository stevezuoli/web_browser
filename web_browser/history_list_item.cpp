#include "web_browser/history_list_item.h"
#include "common/WindowsMetrics.h"
#include <QPainter>
#include "common/debug.h"
using namespace ui::windowsmetrics;

namespace webbrowser
{
HistoryListItem::HistoryListItem(const WebThumbnail& webThumbnail, QListWidget* parent)
    : QWidget(parent)
    , title_label_(new DKLabel(this))
    , url_label_(new DKLabel(this))
    , time_label_(new DKLabel(this))
{
    title_label_->setText(webThumbnail.title());
    url_label_->setText(webThumbnail.url().toString());
    time_label_->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    time_label_->setText(webThumbnail.accessTime());

    main_layout_ = new QHBoxLayout(this);
    right_layout_ = new QVBoxLayout;
    main_layout_->addLayout(right_layout_);
    main_layout_->addSpacing(GetWindowMetrics(HistoryItemLabelMarginIndex));
    main_layout_->addWidget(time_label_);
    right_layout_->addWidget(title_label_);
    right_layout_->addWidget(url_label_);

    setLayout(main_layout_);
}

HistoryListItem::HistoryListItem(const QString& date, QListWidget* parent)
    : QWidget(parent)
    , date_(date)
{
}

void HistoryListItem::paintEvent(QPaintEvent* event)
{
    if (!date_.isNull())
    {
        QPainter painter(this);
        QFont textFont = font();
        textFont.setPixelSize(GetWindowFontSize(HistoryDateIndex));
        setFont(textFont);
        painter.drawText(rect(), Qt::AlignLeft | Qt::AlignVCenter, date_);
        painter.drawLine(0, 0, width(), 0);
    }
    QWidget::paintEvent(event);
}
}
