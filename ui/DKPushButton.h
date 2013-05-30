#ifndef __UI_DKPUSHBUTTON_H__
#define __UI_DKPUSHBUTTON_H__

#include <QPushButton>
namespace ui
{
class DKPushButton : public QPushButton
{
    Q_OBJECT

public:
    DKPushButton(QWidget* parent = 0);
    DKPushButton(const QString& text, QWidget* parent = 0);
    DKPushButton(const QIcon& icon, const QString& text, QWidget* parent = 0);

    void setBackGroundImagePaths(const QString& focusInPath, const QString& focusOutPath)
    {
        m_focusInBackgroudImagePath = focusInPath;
        m_focusOutBackgroudImagePath = focusOutPath;
    }
protected:
    virtual void paintEvent(QPaintEvent*);

private:
    void setDKStyleSheet();
    void InitDKProperty();
    QString m_focusInBackgroudImagePath;
    QString m_focusOutBackgroudImagePath;
};//DKPushButton

}
#endif//__UI_DKPUSHBUTTON_H__
