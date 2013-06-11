#ifndef BOOKMARK_MODEL_H
#define BOOKMARK_MODEL_H

#include <QtGui/QtGui>

namespace webbrowser
{

class BookmarkModel
{
public:
    BookmarkModel();
    ~BookmarkModel();

    bool saveBookmarks();
    bool loadBookmarks();
    bool addBookmark(const QString & title, const QString & url);
    bool deleteBookmark(const QString & title, const QString & url);
    bool hasBookmark(const QString & title, const QString & url);
    void getBookmarksModel(QStandardItemModel & bookmarks_model);

private:
    QString             path_;
    bool                need_save_bookmarks_;
};

};

#endif
