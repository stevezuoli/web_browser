#include <QtGui/QtGui>
#include "buttons.h"
#include "Screen/screen_proxy.h"

static const QString STYLE = "                          \
QCheckBox {                                             \
    padding : 10px;                                     \
    font-size: 18px;                                    \
    background: white;                                  \
    border-width: 1px;                                  \
    border-color: gray;                                 \
    border-style: solid;                                \
    border-radius: 3;                                   \
 }                                                      \
 QCheckBox:checked {                                    \
    background: #a3a3a3;                                \
 }                                                      \
 QCheckBox:focus {                                      \
    border-width: 2px;                                  \
    border-color: black;                                 \
    border-style: solid;                                \
    border-radius: 3;                                   \
 }                                                      \
 QCheckBox::indicator {                                 \
     width: 25px;                                       \
     height: 25px;                                      \
 }                                                      \
 QCheckBox::indicator:unchecked {                       \
     image: url(:/images/check_box_normal.png);         \
 }                                                      \
 QCheckBox::indicator:unchecked:hover {                 \
     image: url(:/images/check_box_normal.png);         \
 }                                                      \
 QCheckBox::indicator:unchecked:pressed {               \
     image: url(:/images/check_box_normal.png);         \
 }                                                      \
 QCheckBox::indicator:checked {                         \
     image: url(:/images/check_box_checked.png);        \
 }                                                      \
 QCheckBox::indicator:checked:hover {                   \
     image: url(:/images/check_box_checked.png);        \
 }                                                      \
 QCheckBox::indicator:checked:pressed {                 \
     image: url(:/images/check_box_checked.png);        \
 }                                                      \
";


static const QString PUSH_BUTTON_STYLE =   "\
QPushButton                             \
{                                       \
    background: white;                  \
    font-size: 18px;                    \
    border-width: 1px;                  \
    border-color: black;                \
    border-style: solid;                \
    border-radius: 3;                   \
    color: black;                       \
    padding: 0px;                       \
    min-width: 65px;                    \
    min-height: 32px;                   \
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
    border-color: white;                \
    background-color: black;            \
}                                       \
QPushButton:focus {                     \
    border-width: 2px;                  \
    border-color: black;                \
    border-style: solid;                \
    border-radius: 3;                   \
}                                       \
QPushButton:disabled                    \
{                                       \
    padding-left: 0px;                  \
    padding-top: 0px;                   \
    border-color: dark;                 \
    color: dark;                        \
    background-color: white;            \
}";


static const QString PUSH_BUTTON_ICON_STYLE =   "\
QPushButton                             \
{                                       \
    background: transparent;            \
    border-width: 0px;                  \
    border-color: transparent;          \
    border-style: solid;                \
    border-radius: 3;                   \
    color: black;                       \
    padding: 0px;                       \
    min-height: 32px;                   \
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
    border-color: white;                \
    background-color: black;            \
}                                       \
QPushButton:focus                       \
{                                       \
    padding-left: 0px;                  \
    padding-top: 0px;                   \
    color: black;                       \
    border-width: 2px;                  \
    border-color: black;                \
    background-color: white;            \
}                                       \
QPushButton:disabled                    \
{                                       \
    padding-left: 0px;                  \
    padding-top: 0px;                   \
    border-color: dark;                 \
    color: dark;                        \
    background-color: white;            \
}";

namespace framework
{

DuokanCheckBox::DuokanCheckBox(const QString & text, QWidget *parent)
: QCheckBox(text, parent)
, select_on_clicked_(true)
{
    setStyleSheet(STYLE);
    setAutoExclusive(true);
    setCheckable(true);
    useDefaultHeight();
}

DuokanCheckBox::~DuokanCheckBox()
{
}

void DuokanCheckBox::selectOnClicked(bool enable)
{
    select_on_clicked_ = enable;
    if (!enable)
    {
        setAutoExclusive(false);
    }
}

bool DuokanCheckBox::event(QEvent *e)
{
    switch (e->type())
    {
    case QEvent::MouseMove:
    case QEvent::HoverMove:
    case QEvent::HoverEnter:
    case QEvent::HoverLeave:
        e->accept();
        return true;
    default:
        break;
    }
    return QCheckBox::event(e);
}

void DuokanCheckBox::mousePressEvent(QMouseEvent *event)
{
    if (isSelectOnClicked())
    {
        event->accept();
    }
    else
    {
        QCheckBox::mousePressEvent(event);
    }
}

void DuokanCheckBox::mouseReleaseEvent(QMouseEvent *event)
{
    if (isSelectOnClicked())
    {
        if (!isChecked())
        {
            setChecked(true);
        }
    }
    else
    {
        setChecked(!isChecked());
        event->accept();
        // QCheckBox::mouseReleaseEvent(event);
    }
    emit clicked(true);
}

void DuokanCheckBox::keyPressEvent(QKeyEvent * event)
{
    event->ignore();
}

int DuokanCheckBox::defaultHeight()
{
    return 42;
}

void DuokanCheckBox::useDefaultHeight()
{
    setFixedHeight(defaultHeight());
}

void DuokanCheckBox::keyReleaseEvent(QKeyEvent * event)
{
    switch (event->key())
    {
    case Qt::Key_Enter:
    case Qt::Key_Return:
        event->accept();
        click();
        break;
    default:
        break;
    }
    event->ignore();
}

DuokanPushButton::DuokanPushButton(const QString & title, QWidget *parent)
: QPushButton(title, parent)
, screen_update_(true)
{
    setStyleSheet(PUSH_BUTTON_STYLE);
    setFocusPolicy(Qt::TabFocus);
}

DuokanPushButton::DuokanPushButton(const QIcon & icon, const QString & text, QWidget * parent)
: QPushButton(icon, text, parent)
, screen_update_(true)
{
    setStyleSheet(PUSH_BUTTON_ICON_STYLE);
    setFocusPolicy(Qt::TabFocus);
    setIconSize(icon.actualSize(QSize(50, 50)));
    setFixedWidth(icon.actualSize(QSize(50, 50)).width() + 8);
}

DuokanPushButton::~DuokanPushButton()
{
}

void DuokanPushButton::setData(const QVariant & data)
{
    data_ = data;
}

QVariant & DuokanPushButton::data()
{
    return data_;
}

const QString & DuokanPushButton::defaultStyle()
{
    return PUSH_BUTTON_STYLE;
}

void DuokanPushButton::useDefaultHeight()
{
    setFixedHeight(defaultHeight());
}

int DuokanPushButton::defaultHeight()
{
    return 36;
}

void DuokanPushButton::mousePressEvent(QMouseEvent *me)
{
    if (screen_update_)
    {
        duokan::screen::instance().enableUpdate(false);
    }

    QPushButton::mousePressEvent(me);

    if (screen_update_)
    {
        duokan::screen::instance().enableUpdate(true);
        duokan::screen::instance().updateWidget(this, duokan::screen::ScreenProxy::DW);
    }
}

void DuokanPushButton::mouseReleaseEvent(QMouseEvent *me)
{
    if (screen_update_)
    {
        duokan::screen::instance().enableUpdate(false);
    }

    QPushButton::mouseReleaseEvent(me);

    if (screen_update_)
    {
        duokan::screen::instance().enableUpdate(true);
        duokan::screen::instance().updateWidget(this, duokan::screen::ScreenProxy::DW);
    }
}

void DuokanPushButton::keyPressEvent(QKeyEvent * event)
{
    event->ignore();
}

void DuokanPushButton::keyReleaseEvent(QKeyEvent * event)
{
    switch (event->key())
    {
    case Qt::Key_Enter:
    case Qt::Key_Return:
        event->accept();
        click();
        break;
    default:
        break;
    }
    event->ignore();
}

bool DuokanPushButton::event(QEvent *e)
{
    switch (e->type())
    {
    case QEvent::MouseMove:
    case QEvent::HoverMove:
    case QEvent::HoverEnter:
    case QEvent::HoverLeave:
        e->accept();
        return true;
    default:
        break;
    }
    return QPushButton::event(e);
}


ImageWidget::ImageWidget(QImage image, QWidget *parent)
: QWidget(parent)
, image_(image)
, check_(false)
{
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

ImageWidget::~ImageWidget()
{
}

QSize ImageWidget::sizeHint() const
{
    return image_.size();
}

QSize ImageWidget::minimumSizeHint() const
{
    return sizeHint();
}

void ImageWidget::setChecked(bool check)
{
    if (check_ != check)
    {
        check_ = check;
        update();
    }
}

void ImageWidget::mousePressEvent(QMouseEvent *event)
{
    event->accept();
}

void ImageWidget::mouseReleaseEvent(QMouseEvent *event)
{
    event->accept();
    check_ = true;
    emit clicked(this);
}

void ImageWidget::keyReleaseEvent(QKeyEvent *key)
{
    key->accept();
    if (key->key() == Qt::Key_Return)
    {
        check_ = true;
        emit clicked(this);
    }
}

void ImageWidget::paintEvent(QPaintEvent *pe)
{
    QRect rc = rect();
    QPainter painter(this);
    painter.fillRect(rc, QBrush(QColor(255, 255, 255)));

    QPoint pos((rc.width() - image_.width()) >> 1, (rc.height() - image_.height()) >> 1);
    painter.drawImage(pos, image_);

    if (isChecked())
    {
        QPainterPath path;
        path.addRoundedRect(rc.adjusted(2, 2, -2, -2), 8, 8);
        painter.drawPath(path);
    }
}

}


