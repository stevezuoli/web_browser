#include "web_browser/history_list_item.h"
#include "common/WindowsMetrics.h"
#include <QPainter>
using namespace ui::windowsmetrics;

namespace webbrowser
{
HistoryListItem::HistoryListItem(const WebThumbnail& webThumbnail, QListWidget* parent)
    : QWidget(parent)
    , title_label_(new DKLabel(this))
    , url_label_(new DKLabel(this))
    , line_pixel_(0)
{
    title_label_->setText(webThumbnail.title());
    url_label_->setText(webThumbnail.url().toString());

    main_layout_ = new QVBoxLayout(this);
    main_layout_->setSpacing(0);
    main_layout_->setContentsMargins(0, 0, 0, 0);
    main_layout_->addWidget(title_label_);
    main_layout_->addSpacing(GetWindowMetrics(HistoryItemSpaceIndex));
    main_layout_->addWidget(url_label_);
    main_layout_->addStretch();

    setLayout(main_layout_);
}

HistoryListItem::HistoryListItem(const QString& date, QListWidget* parent)
    : QWidget(parent)
    , title_label_(NULL)
    , url_label_(NULL)
    , date_(date)
    , line_pixel_(1)
{
}

QString HistoryListItem::getUrl() const
{
    if (url_label_)
    {
        return url_label_->text();
    }

    return "";
}

bool HistoryListItem::isDateItem() const
{
    return !date_.isEmpty();
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
        if (line_pixel_ > 0)
        {
            painter.drawLine(0, 0, width(), line_pixel_ - 1);
        }
    }
    QWidget::paintEvent(event);
}

void HistoryListItem::elideTextWithWidth(int w)
{
    if (url_label_ && title_label_)
    {
        QFont font(url_label_->font());
        font.setPixelSize(GetWindowFontSize(DKLabelIndex));
        QFontMetrics fm(font);
        url_label_->setText(fm.elidedText(url_label_->text(), Qt::ElideRight, w));
        title_label_->setText(fm.elidedText(title_label_->text(), Qt::ElideRight, w));
    }
}
}
