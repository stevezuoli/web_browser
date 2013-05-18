#include "keyboard_dialog.h"

namespace webbrowser
{

KeyboardDialog::KeyboardDialog(QWidget *parent)
    : QDialog(parent)
    , hbox_()
{
    createLayout();

    // Widget attributes.
    setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
    setModal(true);
    setFocusPolicy(Qt::NoFocus);
}

KeyboardDialog::~KeyboardDialog(void)
{
}

void KeyboardDialog::createLayout()
{

}

void KeyboardDialog::ensureVisible()
{
    if (isHidden())
    {
        show();
    }

    QRect parent_rect = parentWidget()->rect();
    int width = parent_rect.width();
    if (size().width() != width)
    {
        resize(width, height());
    }

    adjustPosition();

}

QSize KeyboardDialog::sizeHint () const
{
    return QSize(-1, -1);
}

QSize KeyboardDialog::minimumSize () const
{
    return sizeHint();
}

QSize KeyboardDialog::maximumSize () const
{
    return sizeHint();
}

void KeyboardDialog::mouseMoveEvent(QMouseEvent *me)
{
    me->accept();
}

void KeyboardDialog::mousePressEvent(QMouseEvent *me)
{
    me->accept();
}

void KeyboardDialog::mouseReleaseEvent(QMouseEvent *me)
{
    me->accept();
    qDebug("Mouse Release on KeyboardDialog");
    if (!rect().contains(me->pos()))
    {
        reject();
    }
}

void KeyboardDialog::keyReleaseEvent(QKeyEvent *ke)
{
    int key = ke->key();
    if (key == Qt::Key_Escape)
    {
        onClose();
        return;
    }
}

/// The keyPressEvent could be sent from virtual keyboard.
void KeyboardDialog::keyPressEvent(QKeyEvent * ke)
{
    ke->accept();
    if (ke->key() == Qt::Key_Enter ||
        ke->key() == Qt::Key_Left ||
        ke->key() == Qt::Key_Right ||
        ke->key() == Qt::Key_Return ||
        ke->key() == Qt::Key_F23 ||
        ke->key() == Qt::Key_Shift ||
        ke->key() == Qt::Key_CapsLock)
    {
        return;
    }

    // Disable the parent widget to update screen.
    QKeyEvent * key_event = new QKeyEvent(ke->type(), ke->key(), ke->modifiers(), ke->text());

    QApplication::processEvents();

}

void KeyboardDialog::adjustPosition()
{
    int x = 0;
    int y = parentWidget()->height() - height();
    move(x, y);
}

bool KeyboardDialog::event(QEvent * event)
{
    return QDialog::event(event);;
}

void KeyboardDialog::moveEvent(QMoveEvent *e)
{
    QDialog::moveEvent(e);
}

void KeyboardDialog::resizeEvent(QResizeEvent *e)
{
    QDialog::resizeEvent(e);
}

void KeyboardDialog::clearText()
{
}

void KeyboardDialog::onTimeout()
{
}

void KeyboardDialog::onOKClicked()
{
}

void KeyboardDialog::onClose()
{
    done(QDialog::Rejected);
}

}
