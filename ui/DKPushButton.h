#ifndef __UI_DKPUSHBUTTON_H__
#define __UI_DKPUSHBUTTON_H__

#include <QPushButton>

class DKPushButton : public QPushButton
{
public:
    DKPushButton(QWidget* parent = 0);
    DKPushButton(const QString& text, QWidget* parent = 0);
    DKPushButton(const QIcon& icon, const QString& text, QWidget* parent = 0);
};//DKPushButton

#endif//__UI_DKPUSHBUTTON_H__
