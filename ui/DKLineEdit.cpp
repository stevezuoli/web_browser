#include "ui/DKLineEdit.h"    

namespace ui
{
const QString LINE_EDIT_STYLE = "       \
QLineEdit                               \
{                                       \
    border: 0px solid dark;             \
    background: white;                  \
    selection-background-color: black;  \
    selection-color: white;             \
    font: 24px bold;                    \
    color: black;                       \
    padding: 2px;                       \
}                                       \
QLineEdit:disabled                      \
{                                       \
    border: 0px solid dark;             \
    background: white;                  \
    selection-background-color: black;  \
    selection-color: white;             \
    font: 24px bold;                    \
    color: dark;                       \
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
    emit focusIn();
    QLineEdit::focusInEvent(e);
}

void DKLineEdit::focusOutEvent(QFocusEvent* e)
{
    emit focusOut();
    QLineEdit::focusOutEvent(e);
}
}
