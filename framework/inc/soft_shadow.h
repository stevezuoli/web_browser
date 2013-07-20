#ifndef DUOKAN_SOFT_SHADOW_H_
#define DUOKAN_SOFT_SHADOW_H_

#include <QtGui/QtGui>

namespace framework
{

/// Soft shadow for eink device.
class DuokanShadowWidget : public QDialog
{
public:
    DuokanShadowWidget(QWidget *parent, Qt::Orientation o);
    ~DuokanShadowWidget();

protected:
    void resizeEvent(QResizeEvent *e);
    void paintEvent(QPaintEvent * event);

private:
    QPainterPath getHorizontalPath();
    QPainterPath getVerticalPath();

private:
    Qt::Orientation orient_;
};

/// Reprents horizontal and vertical shadows.
class Shadows : public QObject
{
public:
    Shadows(QWidget *parent);
    ~Shadows();

public:
    void show(bool show = true);
    void onWidgetMoved(QWidget *sibling);
    void onWidgetResized(QWidget *sibling);

    DuokanShadowWidget & hor_shadow() { return hor_shadow_; }
    DuokanShadowWidget & ver_shadow() { return ver_shadow_; }

public:
    static const int PIXELS;

private:
    DuokanShadowWidget hor_shadow_;
    DuokanShadowWidget ver_shadow_;

};

};  // namespace framework

#endif  // DUOKAN_SOFT_SHADOW_H_
