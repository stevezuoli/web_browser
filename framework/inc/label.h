#ifndef DUOKAN_LABEL_H_
#define DUOKAN_LABEL_H_

#include <QtGui/QtGui>

namespace framework
{

/// Label for eink device. Use the stylesheet for eink device.
class DuokanLabel : public QLabel
{
public:
    DuokanLabel(QWidget * parent = 0);
    DuokanLabel(const QString & text, QWidget *parent = 0);
    ~DuokanLabel();

public:
    void useTitleBarStyle();
    void useMessageStyle(int width = 0);
    void squeezeText(Qt::TextElideMode mode = Qt::ElideMiddle);
};

};

#endif  // DUOKAN_LABEL_H_
