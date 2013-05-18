#include "bookmark_model.h"

namespace webbrowser
{

BookmarkModel::BookmarkModel()
    : path_(QDir::homePath())
    , need_save_bookmarks_(false)
{
    QFileInfo file_info(path_, "web_browser");
    path_ = file_info.absoluteFilePath();
    loadBookmarks();
}

BookmarkModel::~BookmarkModel()
{
    saveBookmarks();
}

bool BookmarkModel::saveBookmarks()
{
    return false;
}

bool BookmarkModel::loadBookmarks()
{
    return false;
}

bool BookmarkModel::addBookmark(const QString & title, const QString & url)
{
    return false;
}

bool BookmarkModel::deleteBookmark(const QString & title, const QString & url)
{
    return false;
}

bool BookmarkModel::hasBookmark(const QString & title, const QString & url)
{
    return false;
}

void BookmarkModel::getBookmarksModel(QStandardItemModel & bookmarks_model)
{
}

}
