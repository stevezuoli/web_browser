#include "ui/DKLineEdit.h"    

DKLineEdit::DKLineEdit(QWidget* parent)
    : QLineEdit(parent)
{

}

DKLineEdit::DKLineEdit(const QString& contents, QWidget* parent)
    : QLineEdit(contents, parent)
{
}
