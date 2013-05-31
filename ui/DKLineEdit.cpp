#include "ui/DKLineEdit.h"    
#include "ui/DKSoftKeyboardIME.h"

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
<<<<<<< HEAD
#ifndef WIN32
    qDebug("%s", __PRETTY_FUNCTION__);
#endif
    DKSoftKeyboardIME* ime = DKSoftKeyboardIME::GetInstance();
    if (ime)
    {
        ime->attachReceiver(this);
        ime->show();
    }
=======
    emit focusSignal(true);
    //qDebug("%s", __PRETTY_FUNCTION__);
    //DKSoftKeyboardIME* ime = DKSoftKeyboardIME::GetInstance();
    //if (ime)
    //{
        //ime->attachReceiver(this);
        //ime->show();
    //}
>>>>>>> 6a564a6513cd59e099e52c5d9b521c6e790a537b
    QLineEdit::focusInEvent(e);
}

void DKLineEdit::focusOutEvent(QFocusEvent* e)
{
<<<<<<< HEAD
#ifndef WIN32
    qDebug("%s", __PRETTY_FUNCTION__);
#endif
    DKSoftKeyboardIME* ime = DKSoftKeyboardIME::GetInstance();
    if (ime)
    {
        ime->attachReceiver(NULL);
        ime->hide();
    }
=======
    emit focusSignal(false);
    //qDebug("%s", __PRETTY_FUNCTION__);
    //DKSoftKeyboardIME* ime = DKSoftKeyboardIME::GetInstance();
    //if (ime)
    //{
        //ime->attachReceiver(NULL);
        //ime->hide();
    //}
>>>>>>> 6a564a6513cd59e099e52c5d9b521c6e790a537b
    QLineEdit::focusOutEvent(e);
}
}
