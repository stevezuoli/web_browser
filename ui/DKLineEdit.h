#ifndef __UI_DKLINEEDIT_H__
#define __UI_DKLINEEDIT_H__

#include <QLineEdit>

class DKLineEdit : public QLineEdit
{
public:
    DKLineEdit(QWidget* parent = 0);
    DKLineEdit(const QString& contents, QWidget* parent = 0);
};//DKLineEdit
#endif//__UI_DKLINEEDIT_H__
