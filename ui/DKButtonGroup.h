#ifndef __UI_DKBUTTONGROUP_H__
#define __UI_DKBUTTONGROUP_H__
#include <QWidget>
#include <QHBoxLayout>
#include <vector>

namespace ui
{
class DKPushButton;
class DKButtonGroup : public QWidget
{
public:
    DKButtonGroup(QWidget* parent = 0);
    DKPushButton* addButton(const QString& text);

protected:
    virtual void paintEvent(QPaintEvent* event);

private:
    QHBoxLayout layout_;
    int topline_pixel;
    std::vector<DKPushButton*> buttons_;
};//DKButtonGroup
}//ui
#endif//__UI_DKBUTTONGROUP_H__
