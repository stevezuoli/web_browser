#ifndef WEBBROWSER_HISTORY_PAGE_H_
#define WEBBROWSER_HISTORY_PAGE_H_

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "ui/DKButtonGroup.h"
#include "ui/DKListWidget.h"
#include <vector>

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
    virtual bool eventFilter(QObject* watched, QEvent* event);
    //virtual void keyPressEvent(QKeyEvent* event);
    //virtual void keyReleaseEvent(QKeyEvent* event);

Q_SIGNALS:
    void historyPageQuit(const QUrl&);

private Q_SLOTS:
    void clearHistoryData(bool);
    void onItemClicked(QListWidgetItem* item);
    void onCloseButtonClicked(bool);

private:
    void initLayout();
    void initItems();
    bool onUpDownKeyPressed(bool next);

private:
    DKListWidget history_list_;
    DKButtonGroup bottom_buttons_;
    QVBoxLayout main_layout_;
    BrowserView* view_;
    std::vector<DKPushButton*> bottom_buttons_container;
};
}//webbrowser
#endif//WEBBROWSER_HISTORY_PAGE_H_
