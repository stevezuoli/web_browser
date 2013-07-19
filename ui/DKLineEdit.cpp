#include "ui/DKLineEdit.h"    
#include "ui/DKSoftKeyboardIME.h"
#include "common/debug.h"

namespace ui
{
const QString LINE_EDIT_STYLE = "       \
QLineEdit                               \
{                                       \
    border: 0px solid dark;             \
    selection-background-color: black;  \
    selection-color: white;             \
    font: 24px bold;                    \
    color: black;                       \
    padding: 2px;                       \
}                                       \
QLineEdit:disabled                      \
{                                       \
    border: 0px solid dark;             \
    selection-background-color: black;  \
    selection-color: white;             \
    font: 24px bold;                    \
    color: dark;                        \
    padding: 2px;                       \
}";

DKLineEdit::DKLineEdit(QWidget* parent)
    : QLineEdit(parent)
{
    InitDKProperty();
}

DKLineEdit::DKLineEdit(const QString& contents, QWidget* parent)
    : QLineEdit(contents, parent)
{
    InitDKProperty();
}

void DKLineEdit::InitDKProperty()
{
    setDKStyleSheet();
}

void DKLineEdit::setDKStyleSheet()
{
    setStyleSheet(LINE_EDIT_STYLE);
}

void DKLineEdit::focusInEvent(QFocusEvent* e)
{
    selectall_in_mouse_press_ = true;
    emit focusSignal(true);
    QLineEdit::focusInEvent(e);
}

void DKLineEdit::focusOutEvent(QFocusEvent* e)
{
    emit focusSignal(false);
    QLineEdit::focusOutEvent(e);
    selectall_in_mouse_press_ = false;
}

void DKLineEdit::mousePressEvent(QMouseEvent* event)
{
    if (selectall_in_mouse_press_)
    {
        selectall_in_mouse_press_ = false;
        selectAll();
    }
    else
    {
        QLineEdit::mousePressEvent(event);
    }
}

void DKLineEdit::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_AltGr)
    {
        emit focusSignal(true);
        event->accept();
        return;
    }
    else
    {
        QLineEdit::keyPressEvent(event);
    }
}
}
