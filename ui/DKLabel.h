#ifndef __UI_DKLABEL_H__
#define __UI_DKLABEL_H__

#include <QLabel>
namespace ui
{
class DKLabel : public QLabel
{
    Q_OBJECT

public:
    DKLabel(QWidget* parent = 0);
    DKLabel(const QString& text, QWidget* parent = 0);

    void setFontSize(int fontSize);

private:
    void setDKStyleSheet();
    void InitDKProperty();
};//DKLabel
}
#endif//__UI_DKLABEL_H__
