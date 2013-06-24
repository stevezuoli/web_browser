#ifndef WEBBROWSER_HISTORY_PAGE_H_
#define WEBBROWSER_HISTORY_PAGE_H_

#include <QWidget>
//#include <DKPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include "ui/DKPushButton.h"

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
    void onHistoryPageQuit(bool);

private Q_SLOTS:
    void clearHistoryData(bool);

private:
    void InitLayout();
    void InitItems();

private:
    QListWidget history_list_;
    DKPushButton clear_button_;
    DKPushButton close_button_;
    QVBoxLayout main_layout_;
    QHBoxLayout bottom_layout_;
    BrowserView* view_;
};
}//webbrowser
#endif//WEBBROWSER_HISTORY_PAGE_H_
