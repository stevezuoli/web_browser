#ifndef WEB_BROWSER_CURSOR_NAVIGATOR_H_
#define WEB_BROWSER_CURSOR_NAVIGATOR_H_

#include <QtGui/QtGui>
#include <QtWebKit/QtWebKit>


namespace webbrowser
{

class CursorNavigator : public QObject
{
    Q_OBJECT
public:
    CursorNavigator(QWebView* view);
    virtual ~CursorNavigator();

    // Cursor
    void updateCursor();
    bool moveCursor(int key);
    QRect findCursorElementRect();
    void moveCursorToCenter(int key);

    // Mouse event
    void postMousePressEvent();
    void postMouseReleaseEvent();

Q_SIGNALS:

private Q_SLOTS:
    void onViewLoadFinished(bool ok);
    void onViewLoadStarted();

private:
    QWebView* view_;
    QCursor finger_cursor_;
    QCursor pointer_cursor_;

    QWebElementCollection form_elements_;
    QWebElementCollection link_elements_;
};

};

#endif
