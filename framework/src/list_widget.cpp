
#include <QtGui/QtGui>
#include "list_widget.h"

namespace framework
{

DuokanListWidget::DuokanListWidget(QWidget *parent)
: QWidget(parent)
, selected_(-1)
, first_visible_(0)
, items_per_page_(1)
{
    QFont font(QApplication::font());
    font.setPointSize(20);
    setFont(font);

    setAutoFillBackground(true);
    setBackgroundRole(QPalette::Base);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

DuokanListWidget::~DuokanListWidget()
{
}

void DuokanListWidget::clear()
{
    all_.clear();
    selected_ = -1;
    first_visible_ = 0;
    items_per_page_ = 1;
    update();
}

int DuokanListWidget::pages()
{
    doLayout();
    if (items_per_page_ > 0)
    {
        return all_.size() / items_per_page_;
    }
    return 1;
}

int DuokanListWidget::currentPage()
{
    doLayout();
    int current = first_visible_ / items_per_page_ + 1;
    return current;
}

void DuokanListWidget::addItem(const QString & string)
{
    all_.push_back(string);
}

bool DuokanListWidget::select(const int index)
{
    selected_ =  index;
    update();
    return true;
}

void DuokanListWidget::paintEvent(QPaintEvent * event)
{
    QPainter p(this);

    doLayout();

    int y = margin();
    int h = fontMetrics().height();

    for(int i = first_visible_; i < all_.size() && i < first_visible_ + items_per_page_; ++i)
    {
        const QString & item = all_.at(i);
        if (isSelected(i))
        {
            p.fillRect(QRectF(margin(), y, width() - 2 * margin(), h), palette().shadow());
            p.setPen(QPen(QColor(255, 255, 255)));
        }
        else
        {
            p.setPen(QPen(QColor(0, 0, 0)));
        }
        p.drawText(QRectF(margin(), y, width() - 2 * margin(), h), item);
        y += h;
        y += spacing();
    }
}

void DuokanListWidget::mousePressEvent(QMouseEvent * event)
{
    event->accept();
}

void DuokanListWidget::mouseReleaseEvent(QMouseEvent * event)
{
    int sel = hitTest(event->pos());
    if (sel >= 0 && sel < items_per_page_)
    {
        select(first_visible_ + sel);
        activate();
    }
}

void DuokanListWidget::keyPressEvent(QKeyEvent * event)
{
}

void DuokanListWidget::keyReleaseEvent(QKeyEvent * event)
{
    int key = event->key();
    if (key == Qt::Key_PageDown)
    {
        if (pageDown())
            update();
    }
    else if (key == Qt::Key_PageUp)
    {
        if (pageUp())
            update();
    }
    else if (key == Qt::Key_Enter)
    {
        activate(); // Activate current selected.
    }
}

int DuokanListWidget::margin()
{
    return 5;
}

int DuokanListWidget::spacing()
{
    return 5;
}

/// Calculate the first visible item.
int DuokanListWidget::firstVisibleItem()
{
    return first_visible_;
}

int DuokanListWidget::hitTest(const QPoint & point)
{
    int pos = point.y() - margin();
    int h = fontMetrics().height() + spacing();
    return pos / h;
}

bool DuokanListWidget::isSelected(int index)
{
    return selected_ == index;
}

bool DuokanListWidget::pageUp()
{
    if (first_visible_ <= 0)
    {
        return false;
    }

    first_visible_ -= items_per_page_;
    if (first_visible_ < 0)
    {
        first_visible_ = 0;
    }
    emit positionChanged(currentPage(), pages());
    return true;
}

bool DuokanListWidget::pageDown()
{
    int new_pos = first_visible_ + items_per_page_;
    if (new_pos >= all_.size())
    {
        return false;
    }

    first_visible_ = new_pos;
    emit positionChanged(currentPage(), pages());
    return true;
}

void DuokanListWidget::activate(int select)
{
    if (select < 0)
    {
        emit activated(all_.at(selected_));
    }
    else
    {
        emit activated(all_.at(select));
    }
}

/// We calculate all layout information here.
void DuokanListWidget::doLayout()
{
    int h = fontMetrics().height() + spacing();
    items_per_page_ = (height() - margin() * 2) / h;
    if (items_per_page_ <= 0)
    {
        items_per_page_ = 1;
    }

    // Check position.
    int pos = 0;
    if (first_visible_ > 0)
    {
        pos = first_visible_;
    }
    else if (selected_ > 0)
    {
        pos = selected_;
    }

    int page = pos / items_per_page_;
    first_visible_ = page * items_per_page_;
    if (selected_ < page * items_per_page_ ||
        selected_ >= (page + 1) * items_per_page_)
    {
        selected_ = first_visible_;
    }
}

}
