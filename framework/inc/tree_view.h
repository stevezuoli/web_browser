#ifndef DUOKAN_TREE_VIEW_H_
#define DUOKAN_TREE_VIEW_H_

#include "Base/base.h"
#include <QtGui/QtGui>

namespace framework
{

/// Duokan tree view for eink device.
class DuokanTreeHeaderBar : public QWidget
{
    Q_OBJECT
public:
    DuokanTreeHeaderBar(QWidget *parent, QStandardItemModel * model);
    ~DuokanTreeHeaderBar();

public:
    void setModel(QStandardItemModel * model);
    void setColumnWidth(const QVector<int> &percentages);
    const QVector<int> & columnWidth() { return percentages_; }
    QRect columnRect(int column);
    QStandardItemModel * model() { return model_; }

public Q_SLOTS:
    void clear();

protected:
    virtual void paintEvent(QPaintEvent *);

private:
    QStandardItemModel * model_;
    QVector<int> percentages_;
};

class DuokanTreeView;
class DuokanTreeViewItem : public QWidget
{
    Q_OBJECT
public:
    DuokanTreeViewItem(QWidget *parent, DuokanTreeView & view);
    ~DuokanTreeViewItem();

public:
    void setData(QStandardItem *item);
    QStandardItem * data() { return data_; }

    bool isSelected() { return selected_; }
    void select(bool select);

    bool needRepaint() const { return need_repaint_;}

Q_SIGNALS:
    void pressed(DuokanTreeViewItem *item, const QPoint & press);
    void released(DuokanTreeViewItem *item, const QPoint & release);
    void clicked(DuokanTreeViewItem *item);

private:
    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseReleaseEvent(QMouseEvent *);
    virtual void keyPressEvent(QKeyEvent *);
    virtual void keyReleaseEvent(QKeyEvent *);
    virtual void paintEvent(QPaintEvent *);
    virtual void resizeEvent(QResizeEvent *);

private:
    QStandardItem * data_;
    bool selected_;
    bool need_repaint_;
    DuokanTreeView & view_;
};

/// Tree view for eink device. Remove unnecessary updates.
/// The tree view also implements necessary pagination.
/// The basic idea is to reuse existing QTreeWidget.
/// It creates internal model with only visible paginated items.
/// When location changed, it updates these visible items and
/// updates the internal tree widget.
class DuokanTreeView : public QWidget
{
    Q_OBJECT
public:
    DuokanTreeView(QWidget *parent, QStandardItemModel * model);
    ~DuokanTreeView();

public:
    int  pages();
    bool jumpToPage(int page);
    int  currentPage();
    int  itemsPerPage();

    bool pageUp();
    bool pageDown();

    void showHeader(bool show);
    DuokanTreeHeaderBar & header() { return header_bar_; }

    void setModel(QStandardItemModel * model);
    QStandardItem * item(QStandardItem *item, int col);

public Q_SLOTS:
    void clear();
    bool select(const QModelIndex & index);
    bool select(const QString & title);
    bool setColumnWidth(const QVector<int> &percentages);
    void keyReleaseEvent(QKeyEvent * event);

protected:
    virtual void keyPressEvent(QKeyEvent * event);
    virtual void resizeEvent(QResizeEvent *e);
    virtual void paintEvent(QPaintEvent *);

Q_SIGNALS:
    void activated(const QModelIndex & index);
    void positionChanged(int current, int total);
    void exceed(bool begin);

private Q_SLOTS:
    void onItemClicked(DuokanTreeViewItem *);
    void onItemPressed(DuokanTreeViewItem *, const QPoint &);
    void onItemReleased(DuokanTreeViewItem *, const QPoint &);

private:
    void createLayout();
    void setupInternalModel(QStandardItemModel * model,
                            QStandardItem *item,
                            int level);
    void arrangeItems(int, int, int);
    void updateLayout(const int rows);
    bool selectItem(DuokanTreeViewItem *item);
    QStandardItem * item(int row, int col);
    bool navigate(int offset);

    QString level(int row, int col);

    void updateTreeWidget();
    int first_visible();
    void reportPosition();

    void activate(int select = -1);

private:
    QVBoxLayout layout_;
    QStandardItemModel *model_;
    QGridLayout items_layout_;

    DuokanTreeHeaderBar header_bar_;

    int selected_;      ///< Absolute position.
    int first_visible_; ///< Absolute position.
    int items_per_page_;
    QVector<QStandardItem *> all_items_;

    typedef shared_ptr<DuokanTreeViewItem> ViewItemPtr;
    typedef QVector<ViewItemPtr> ViewItemPtrs;
    typedef QVector<ViewItemPtr>::iterator ViewItemPtrIter;
    ViewItemPtrs views_;

    DuokanTreeViewItem *pressed_item_;
    QPoint pressed_point_;
};

}   // namespace framework

#endif  // DUOKAN_TREE_VIEW_H_
