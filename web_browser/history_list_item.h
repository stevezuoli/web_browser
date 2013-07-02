#ifndef WEBBROWSER_HISTORY_LIST_ITEM_H_
#define WEBBROWSER_HISTORY_LIST_ITEM_H_

#include <QWidget>
#include <QListWidgetItem>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "ui/DKLabel.h"
#include "Database/web_data.h"

using namespace web_database;
using namespace ui;
namespace webbrowser
{
class HistoryListItem : public QWidget
{
public:
    HistoryListItem(const WebThumbnail& webThumbnail, QListWidget* parent = 0);
    HistoryListItem(const QString& date, QListWidget* parent = 0);
    QString getUrl() const;
    bool isDateItem() const;
    void setLinePixel(int pixel)
    {
        line_pixel_ = pixel;
    }
    void elideTextWithWidth(int width);

protected:
    virtual void paintEvent(QPaintEvent* event);

private:
    DKLabel* title_label_;
    DKLabel* url_label_;
    QVBoxLayout* main_layout_;
    QString date_;
    int line_pixel_;
};//HistoryListItem
}//webbrowser
#endif//WEBBROWSER_HISTORY_LIST_ITEM_H_
