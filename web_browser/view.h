#ifndef WEB_BROWSER_VIEW_H_
#define WEB_BROWSER_VIEW_H_

#include <QtGui/QtGui>
#include <QtWebKit/QtWebKit>
#include "Database/web_data.h"
#include "page.h"
#include "bookmark_model.h"
#include "Gesture/scale_gesture_detector.h"

using namespace web_database;

namespace webbrowser
{

typedef QList<QWidget *> QWidgets;

class HoldAutoSaver;
class BrowserView : public QWebView
{
    Q_OBJECT

public:
    BrowserView(QWidget *parent = 0);
    ~BrowserView();

public:
    void attachBookmarkModel(BookmarkModel * model);

    const QRect & updateRect() { return update_rect_; }
    void resetUpdateRect() { update_rect_.setRect(0, 0, 0, 0); }
    bool isLoadingFinished() { return (progress_ >= 100 || progress_ < 0); }

    void onRangeClicked(const int, const int);
    void reportCurrentProcess();
    void myLoad(const QUrl & url);
    int getProgress() const
    {
        return progress_;
    }

    int GetHistoryPageCounts() const;
    int GetCurrentHistoryPageIndex() const;

    QString getHtmlFromMainFrame();

    QVariantList GetSiteList() const
    {
        return site_list_;
    }
    // Clear HistoryData
    void clearHistoryData();

protected:
    virtual QWebView *createWindow(QWebPage::WebWindowType type);
    virtual void mousePressEvent (QMouseEvent * );
    virtual void mouseMoveEvent(QMouseEvent*);
    virtual void mouseReleaseEvent (QMouseEvent *);
    virtual void mouseDoubleClickEvent(QMouseEvent*);
    virtual void keyPressEvent(QKeyEvent *e);
    virtual void keyReleaseEvent(QKeyEvent *ke);
    virtual void contextMenuEvent(QContextMenuEvent * event);
    virtual void closeEvent(QCloseEvent * event);
    virtual bool eventFilter(QObject *obj, QEvent *event);
    virtual void focusOutEvent(QFocusEvent * event);
    //virtual bool focusNextPrevChild(bool next);

Q_SIGNALS:
    void requestOTA(const QUrl & url);
    void focusOut();
    void progressChangedSignal(const int, const int);
    void viewportUpdated(const int, const int, const int);
    //void showHome();

    void inputFormFocused(const QString& form_id,
                          const QString& form_name,
                          const QString& form_action,
                          const QString& input_type,
                          const QString& input_id,
                          const QString& input_name);
    void inputFormLostFocus(void);
    void keyboardKeyPressed();

    // reader mode
    void enableReaderMode(bool);
    
    // update text on linedit
    void displayTextOnAddressEdit(const QString& message);

public Q_SLOTS:
    void formLostFocus (void);
    void formFocused (const QString& form_id,
                      const QString& form_name,
                      const QString& form_action,
                      const QString& input_type,
                      const QString& input_id,
                      const QString& input_name);
    void formFocusedAddValue (const QString& form_id,
                              const QString& form_name,
                              const QString& form_action,
                              const QString& input_type,
                              const QString& input_id,
                              const QString& input_name,
                              const QString& value);
    void selectMouseUp();
    void handleSelectPopup();
    void selectFocus();
    void selectBlur();
    void selectChanged();
    void returnToLibrary();

    // reader mode
    void enterReaderMode(bool is_reader_mode);

    void onScaleBegin();
    void onScaleEnd();
    void onScaling();

private Q_SLOTS:
    void updateActions();
    void popupMenu();

    void changeFontFamily(const QString&);
    void changeFontSize(qreal size);
    void reloadCurrentUrl();

    void onLoadStarted(void);
    void onLoadProgress(int);
    void onLoadFinished(bool);
    void onRepaintRequested(const QRect&);

    void onSelectionChanged();

    void onDownloadRequested(const QNetworkRequest & request);
    void onSavePassword(const QByteArray & data);

    void fastUpdateWidget(QRect &rect);
    bool isWidgetVisible(QWidget * wnd);
    void hideHelperWidget(QWidget * wnd);

    void startDictLookup();
    void stopDictLookup();

    void showSearchWidget();
    void onSearchClosed();
    bool doSearch();

    void enableTextSelection(bool enable = true);
    bool isTextSelectionEnabled();
    void clearSelection();

    void storeUrl();
    void sync();
    QImage thumbnail(const QSize & size);

    void myScroll(int dx, int dy);
    void myScrollTo(const QPoint & p);
    void updateViewportRange();
    QPointF currentOffset() const;
    void accurateScroll(int dx, int dy);

    void populateJavaScriptWindowObject (void);
    void hideGif();
    void hideInlineFrames();
    void hideObjectElements();
    void hideEmbeddedElements();
    void hideScrollbar();

    void clearHistory();

    //void onMicroFocusChanged();
private:
    void addFormsFocusEvent(void);
    void addSelectEvents(void);
    void storeConf(const QUrl & url);
    bool needConfigNetwork();
    void clearFocusWidgets();

    // Bookmarks
    void displayBookmarks();
    bool addBookmark();
    bool deleteBookmark();

    // Passwords
    bool deletePassword();

    // Proxy
    void configureProxy();

    // Clear Cookies
    void clearCookies();
    
    // Navigation
    QWebElement searchElement(QWebElement element, bool func(const BrowserView* view, const QWebElement& e));
    static bool isInViewport(const BrowserView* view, const QWebElement& element);
    bool isformFocused();

    // Scaling
    bool zoom(qreal zoom_span, int focus_x, int focus_y);

private:
    QPoint position_;
    QPoint offset_;

    int scrollbar_hidden_;         ///< Flag to hide scrollbar.
    int progress_;

    QRect update_rect_;

    //FontFamilyActions font_family_actions_;
    QVariantList site_list_;

    QRect selected_rect_;
    QRect refresh_rect_;
    QString selected_text_;
    bool enable_text_selection_;

    // web data
    bool need_save_url_;
    QUrl current_url_;
    WebData db_;

    QString jquery_;
    QString readability_;
    WebPage *page_;
    BookmarkModel *bookmark_model_;
    QWidgets focus_widgets_;
    QWebElementCollection form_elements_;

    bool hand_tool_enabled_;

    // scaling
    gesture::ScaleGestureContext scale_context_;

private:
    friend class HoldAutoSaver;
};

class HoldAutoSaver
{
public:
    HoldAutoSaver(BrowserView *view) : view_(view) { view_->need_save_url_ = false; }
    ~HoldAutoSaver() { view_->need_save_url_ = true; }
private:
    BrowserView * view_;
};

};   // namespace webbrowser

#endif
