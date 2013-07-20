#ifndef DUOKAN_LIST_WIDGET_H_
#define DUOKAN_LIST_WIDGET_H_

#include <QtGui/QtGui>

namespace framework
{

/// List widget for eink device. Remove unnecessary updates and add pagination.
class DuokanListWidget : public QWidget
{
    Q_OBJECT
public:
    DuokanListWidget(QWidget *parent);
    ~DuokanListWidget();

public:
    void clear();

    int pages();
    int currentPage();

    void addItem(const QString & string);
    bool select(const int index);

protected:
    virtual void paintEvent(QPaintEvent * event);
    virtual void mousePressEvent(QMouseEvent * event);
    virtual void mouseReleaseEvent(QMouseEvent * event);
    virtual void keyPressEvent(QKeyEvent * event);
    virtual void keyReleaseEvent(QKeyEvent * event);

Q_SIGNALS:
    void activated(const QString& text);
    void positionChanged(int current, int total);

private:
    void doLayout();

    int margin();
    int spacing();

    int firstVisibleItem();
    int hitTest(const QPoint & point);

    bool isSelected(int index);
    bool pageUp();
    bool pageDown();

    void activate(int select = -1);

private:
    int selected_;      ///< Single selection.
    int first_visible_;
    int items_per_page_;
    QVector<QString> all_;
};

};

#endif  // DUOKAN_LIST_WIDGET_H_
