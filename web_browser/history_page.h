#ifndef WEBBROWSER_HISTORY_PAGE_H_
#define WEBBROWSER_HISTORY_PAGE_H_

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "ui/DKButtonGroup.h"
#include "ui/DKListWidget.h"

using namespace ui;
namespace webbrowser
{
class BrowserView;
class HistoryPage : public QWidget
{
    Q_OBJECT
public:
    HistoryPage(BrowserView* view, QWidget* parent = 0);
    void setViewer(BrowserView* view)
    {
        view_ = view;
    }
    virtual void setVisible(bool visible);

protected:
    virtual void paintEvent(QPaintEvent* event);

Q_SIGNALS:
    void historyPageQuit(const QString&);

private Q_SLOTS:
    void clearHistoryData(bool);
    void onItemClicked(QListWidgetItem* item);
    void onCloseButtonClicked(bool);

private:
    void InitLayout();
    void InitItems();

private:
    DKListWidget history_list_;
    DKButtonGroup bottom_buttons_;
    QVBoxLayout main_layout_;
    QHBoxLayout bottom_layout_;
    BrowserView* view_;
};
}//webbrowser
#endif//WEBBROWSER_HISTORY_PAGE_H_
