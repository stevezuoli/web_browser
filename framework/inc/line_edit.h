#ifndef DUOKAN_LINE_EDIT_H_
#define DUOKAN_LINE_EDIT_H_

#include <QtGui/QtGui>

namespace framework
{

/// Line edit for eink device. Remove unnecessary updates.
class DuokanLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    DuokanLineEdit(QWidget *parent);
    DuokanLineEdit(const QString & text, QWidget *parent);
    ~DuokanLineEdit();

protected:
    void focusInEvent(QFocusEvent *e);

Q_SIGNALS:
    void getFocus(DuokanLineEdit *object);
    void setCheckByMouse(DuokanLineEdit *object);
    void outOfRange(QKeyEvent *ke);

protected:
    void keyReleaseEvent(QKeyEvent *ke);
    void keyPressEvent(QKeyEvent * ke);
    void mouseReleaseEvent(QMouseEvent * event);

private:
    bool out_of_range_;
};

};

#endif  // DUOKAN_LINE_EDIT_H_
