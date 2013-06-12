#include <QtGui/QtGui>
#include "NetworkService/auto_complete.h"
#include "NetworkService/password_model.h"
#include "Gesture/scale_gesture_detector.h"

#include "view.h"
#include "web_application.h"
#include "common/debug.h"

using namespace network_service;
using namespace gesture;

namespace webbrowser
{

static const int PAGE_REPEAT = 20;
static const int DELTA = 10;
static const qreal DEFAULT_FONT = 1.0;

BrowserView::BrowserView(QWidget *parent)
    : QWebView(parent)
    , scrollbar_hidden_(0)
    , progress_(0)
    , enable_text_selection_(false)
    , need_save_url_(true)
    , page_(new WebPage(this))
    , bookmark_model_(0)
    , hand_tool_enabled_(true)
    , is_current_page_readable_(false)
{
    setPage(page_);

    // In order to receive link clicked event.
    page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);

    // Setup connections.
    connect(this, SIGNAL(loadStarted(void)), this, SLOT(onLoadStarted(void)));
    connect(this, SIGNAL(loadFinished(bool)), this, SLOT(onLoadFinished(bool)));
    connect(this, SIGNAL(loadProgress(int)), this, SLOT(onLoadProgress(int)));
    connect(page(), SIGNAL(repaintRequested(const QRect &)), this, SLOT(onRepaintRequested(const QRect&)));
    connect(page(), SIGNAL(loadingUrl(const QUrl&)), this, SIGNAL(urlChanged(const QUrl &)));
    connect(page(), SIGNAL(downloadRequested(const QNetworkRequest &)), this,
            SLOT(onDownloadRequested(const QNetworkRequest &)));

    connect(page()->mainFrame(), SIGNAL(javaScriptWindowObjectCleared()),
            this, SLOT(populateJavaScriptWindowObject()));
    connect(page_, SIGNAL(requestOTA(const QUrl&)), SIGNAL(requestOTA(const QUrl&)));

    // Use queued connection to make sure selected text is reported correctly.
    connect(this, SIGNAL(selectionChanged()), this, SLOT(onSelectionChanged()),
            Qt::QueuedConnection);

    connect(WebApplication::accessManager(), SIGNAL(requestSavePassword(const QByteArray &)),
            this, SLOT(onSavePassword(const QByteArray &)));

    // Connect scale gesture detector
    connect(ScaleGestureDetector::instance(), SIGNAL(scaleBegin()), this, SLOT(onScaleBegin()));
    connect(ScaleGestureDetector::instance(), SIGNAL(scaleEnd()), this, SLOT(onScaleEnd()));
    connect(ScaleGestureDetector::instance(), SIGNAL(scaling()), this, SLOT(onScaling()));

    settings()->setAttribute(QWebSettings::JavascriptEnabled, true);
    settings()->setAttribute(QWebSettings::JavaEnabled, true);

    page()->setForwardUnsupportedContent(true);

#ifdef USE_JQUERY
    QFile file;
    file.setFileName(":/res/jquery.min.js");
    if (file.open(QIODevice::ReadOnly))
    {
        jquery_ = file.readAll();
        file.close();
    }

#endif
    QFile readability_file;
    readability_file.setFileName(":/res/readability.js");
    if (readability_file.open(QIODevice::ReadOnly))
    {
        readability_ = readability_file.readAll();
        readability_file.close();
    }

    setTextSizeMultiplier(DEFAULT_FONT);
}

BrowserView::~BrowserView()
{
}

/// Seems Qt 4.7 contains problem when it cannot reach network.
/// The url will not be updated.
void BrowserView::myLoad(const QUrl & url)
{
    current_url_ = url;
    load(url);
}

void BrowserView::attachBookmarkModel(BookmarkModel * model)
{
    bookmark_model_ = model;
}

void BrowserView::hideGif()
{
    QString code = "$('[src*=gif]').hide()";
    page()->mainFrame()->evaluateJavaScript(code);
}

void BrowserView::hideInlineFrames()
{
    QString code = "$('iframe').remove()";
    page()->mainFrame()->evaluateJavaScript(code);
}

void BrowserView::hideObjectElements()
{
    QString code = "$('object').remove()";
    page()->mainFrame()->evaluateJavaScript(code);
}

void BrowserView::hideEmbeddedElements()
{
    QString code = "$('embed').remove()";
    page()->mainFrame()->evaluateJavaScript(code);
}

void BrowserView::hideScrollbar()
{
    QString code = "$('body').css('overflow', 'hidden')";
    page()->mainFrame()->evaluateJavaScript(code);
    
}

void BrowserView::onLoadStarted(void)
{
    qDebug("Load Start");

    // In order to use javascript to hide scrollbar.
    scrollbar_hidden_ = 0;
    progress_ = 0;

    // Experimental, set handle tool to be true
    hand_tool_enabled_ = true;
    setIsArticlePage(false);

    // Store the screen update type.
    resetUpdateRect();
    reportCurrentProcess();
    form_elements_ = QWebElementCollection();
}

void BrowserView::onSavePassword(const QByteArray & data)
{
    QWebFrame * current_frame = page()->currentFrame();
    if (current_frame == 0)
    {
        return;
    }

    QUrl url = current_frame->url();
    AutoComplete::instance()->setFormData(url, data);

    // setFormHtml at this moment?
    if (!AutoComplete::instance()->existInForms(url))
    {
        AutoComplete::instance()->setFormHtml(url, current_frame->toHtml());
    }
    AutoComplete::instance()->evaluate(url);
}

void BrowserView::onLoadFinished(bool ok)
{
    // Restore the screen update type.
    // Ensure we can get a gc update.
    progress_ = 100;
    update_rect_ = rect();
    setIsArticlePage(false);

    // Check if we need to store the thumbnail.
    if (ok)
    {
        storeUrl();

        // Auto complete the form in web
        if (WebApplication::accessManager()->autoComplete())
        {
            AutoComplete::instance()->complete(page()->mainFrame());
        }

#ifdef USE_JQUERY
        page()->mainFrame()->evaluateJavaScript(jquery_);
        //hideGif();
        //hideInlineFrames();
        //hideObjectElements();
        //hideEmbeddedElements();
        hideScrollbar();
#endif
        // The keyboard can only be display after load finished
        addFormsFocusEvent();
        addSelectEvents();

        // for reader mode
        askForArticlePage();
    }
    else
    {
        // Failed.
        progress_ = -1;
        reportCurrentProcess();
    }
    updateViewportRange();

    //collect the form elements in current page
    //form_elements_ = page()->currentFrame()->documentElement().findAll("input[type=text],input[type=password], textarea");
    //qDebug() << form_elements_.count();
    //foreach (QWebElement formElement, form_elements_)
    //{
    //    qDebug() << formElement.toOuterXml();
    //}
}

bool BrowserView::needConfigNetwork()
{
    QNetworkReply::NetworkError error = page_->networkError();
    return (error == QNetworkReply::ConnectionRefusedError ||
            error == QNetworkReply::HostNotFoundError ||
            error == QNetworkReply::TimeoutError ||
            error == QNetworkReply::UnknownNetworkError);
}

void BrowserView::onRepaintRequested(const QRect& rc)
{
    update_rect_ = update_rect_.united(rc);
    if (isTextSelectionEnabled())
    {
        selected_rect_ = rc;

        refresh_rect_ = refresh_rect_.unite(rc);
        refresh_rect_ = refresh_rect_.intersect(rect());
    }
}


void BrowserView::onDownloadRequested(const QNetworkRequest & request)
{
}

void BrowserView::reloadCurrentUrl()
{
    if (current_url_.isValid())
    {
        load(current_url_);
    }
}


void BrowserView::updateActions()
{
}

void BrowserView::onRangeClicked(const int percentage,
                                 const int value)
{
    int height = page()->mainFrame()->contentsSize().height();
    QPointF pt = currentOffset();
    pt.ry() = height * percentage / 100 - rect().height();
    myScrollTo(pt.toPoint());
    updateViewportRange();
}

void BrowserView::reportCurrentProcess()
{
    emit progressChangedSignal(progress_, 100);
}

/// Popup the menu.
void BrowserView::popupMenu()
{
}

void BrowserView::returnToLibrary()
{
    sync();
    stop();
    close();
    qApp->exit();
    ::exit(0);
}

void BrowserView::changeFontFamily(const QString & family)
{
    // It does not work for documents that specify font already.
    QWebSettings * settings = QWebSettings::globalSettings();
    if (settings->fontFamily(QWebSettings::StandardFont) == family)
    {
        update(rect());
        return;
    }

    for(int i = static_cast<int>(QWebSettings::StandardFont);
        i <= static_cast<int>(QWebSettings::FantasyFont); ++i)
    {
        settings->setFontFamily(static_cast<QWebSettings::FontFamily>(i), family);
    }
    update(rect());
    updateViewportRange();
}


void BrowserView::configureProxy()
{
}

void BrowserView::changeFontSize(qreal font_size)
{
    if (font_size == textSizeMultiplier())
    {
        update(rect());
        return;
    }

    // Record current offset.
    QSize size = page()->mainFrame()->contentsSize();
    QPointF pt = currentOffset();

    // Change size now.
    setTextSizeMultiplier(font_size);

    // Scroll to there again.
    QSize new_size = page()->mainFrame()->contentsSize();
    pt.rx() = pt.x() * size.width() / new_size.width();
    pt.ry() = pt.y() * size.height() / new_size.height();
    myScrollTo(pt.toPoint());
}

QWebView *BrowserView::createWindow(QWebPage::WebWindowType type)
{
    return this;
}

void BrowserView::mousePressEvent(QMouseEvent*me)
{
    emit inputFormLostFocus();
    position_ = me->pos();

    return QWebView::mousePressEvent(me);
}

void BrowserView::mouseMoveEvent(QMouseEvent *me)
{
    if (!hand_tool_enabled_)
    {
        QWebView::mouseMoveEvent(me);
    }
    else
    {
        me->accept();
    }
}

void BrowserView::mouseReleaseEvent(QMouseEvent*me)
{
    QPoint delta = position_ - me->pos();
    if (isTextSelectionEnabled())
    {
        QWebView::mouseReleaseEvent(me);
        QContextMenuEvent ce(QContextMenuEvent::Mouse,
                             me->pos(),
                             me->globalPos(),
                             me->modifiers());
        page()->swallowContextMenuEvent(&ce);
        emit selectionChanged();
        return;
    }

    if (abs(delta.x()) < DELTA && abs(delta.y()) < DELTA)
    {
        // Could click a link.
        QWebView::mouseReleaseEvent(me);
        return;
    }

    // Pan.
    resetUpdateRect();

    if (!hand_tool_enabled_)
    {
        QWebView::mouseReleaseEvent(me);
    }
    else
    {
        me->accept();
        accurateScroll(delta.x(), delta.y());
    }
}

void BrowserView::keyPressEvent(QKeyEvent *e)
{
    // We only handle key release event, so ignore some keys.
    switch (e->key())
    {
    case Qt::Key_Down:
    case Qt::Key_Up:
        break;
    case Qt::Key_AltGr:
        if (isformFocused())
        {
            emit keyboardKeyPressed();
        }
        break;
    default:
        QWebView::keyPressEvent(e);
        break;
    }
    e->accept();
}

void BrowserView::keyReleaseEvent(QKeyEvent *ke)
{
    DebugWB("%d, %s, %d", ke->key(), qPrintable(ke->text()), int(ke->modifiers()));
    switch (ke->key())
    {
    case Qt::Key_Left:
        myScroll(0, -PAGE_REPEAT);
        break;
    case Qt::Key_Right:
        myScroll(0, PAGE_REPEAT);
        break;
    case Qt::Key_PageDown:
        myScroll(0, rect().height() - PAGE_REPEAT);
        break;
    case Qt::Key_Down:
        focusNextPrevChild(true);
        break;
    case Qt::Key_PageUp:
        myScroll(0, -rect().height() + PAGE_REPEAT);
        break;
    case Qt::Key_Up:
        focusNextPrevChild(false);
        break;
    case Qt::Key_C:
        clearCookies();
        break;
    case Qt::Key_R:
        enterReaderMode(true);
        break;
    default:
        QWebView::keyReleaseEvent(ke);
        break;
    }
    ke->ignore();
}

void BrowserView::contextMenuEvent(QContextMenuEvent * event)
{
    event->accept();
}

void BrowserView::closeEvent(QCloseEvent * event)
{
    storeConf(url());
    QWebView::closeEvent(event);
}

/// Ignore the double click event.
void BrowserView::mouseDoubleClickEvent(QMouseEvent*me)
{
    me->accept();
    // return QWebView::mouseDoubleClickEvent(me);
}

/// Stores the url if it's not in database yet. It stores thumbnail too.
void BrowserView::storeUrl()
{
    const QString & myurl = url().toString();
    if (!myurl.startsWith("http",Qt::CaseInsensitive) && !myurl.startsWith("ftp",Qt::CaseInsensitive) && !myurl.startsWith("www",Qt::CaseInsensitive))
    {
        return ;
    }

    sync();
}

void BrowserView::saveThumbnailForExplorer()
{
}

void BrowserView::sync()
{
}

void BrowserView::saveThumbnails()
{
}

QImage BrowserView::thumbnail(const QSize & size)
{
    QImage image(page()->viewportSize(), QImage::Format_ARGB32);
    QPainter painter(&image);
    page()->mainFrame()->render(&painter);

    return image.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
}

void BrowserView::startDictLookup()
{
}

void BrowserView::stopDictLookup()
{
}

void BrowserView::onSelectionChanged()
{
    QString text = selectedText();
    if (selected_text_ == text)
    {
        return;
    }

    selected_text_ = text;
}

void BrowserView::showSearchWidget()
{
}

void BrowserView::onSearchClosed()
{
    // Clear all selected text.
    clearSelection();
}

bool BrowserView::doSearch()
{
    return false;
}

void BrowserView::enableTextSelection(bool enable)
{
    enable_text_selection_ = enable;
}

bool BrowserView::isTextSelectionEnabled()
{
    return enable_text_selection_;
}

void BrowserView::fastUpdateWidget(QRect &rect)
{
    repaint();
}

bool BrowserView::isWidgetVisible(QWidget * wnd)
{
    if (wnd)
    {
        return wnd->isVisible();
    }
    return false;
}

void BrowserView::hideHelperWidget(QWidget * wnd)
{
    if (wnd)
    {
        wnd->hide();
    }
}

/// Clear highlight text by using javascript.
void BrowserView::clearSelection()
{
    page()->mainFrame()->evaluateJavaScript("document.selection.empty;");
    page()->mainFrame()->evaluateJavaScript("window.getSelection().removeAllRanges();");
}

void BrowserView::accurateScroll(int dx, int dy)
{
    page()->currentFrame()->scroll(dx, dy);
    updateViewportRange();
}

void BrowserView::myScroll(int dx, int dy)
{
    page()->currentFrame()->scroll(dx, dy);
    updateViewportRange();

    QWebElement element = searchElement(page()->currentFrame()->documentElement(), isInViewport);
    if (!element.isNull())
    {
        element.setFocus();
    }
}

QWebElement BrowserView::searchElement(QWebElement element, bool func(const BrowserView* view, const QWebElement& e))
{
    QWebElement child = element.firstChild();

    if (child.isNull() && func(this, element))
    {
        return element;
    }
    while (!child.isNull())
    {
        QWebElement result = searchElement(child, func);
        if (!result.isNull())
        {
            return result;
        }
        child = child.nextSibling();
    }

    return QWebElement();
}

bool BrowserView::isInViewport(const BrowserView* view, const QWebElement& element)
{
    QRect rc = element.geometry();
    return !element.isNull()
        && rc.isValid()
        && (rc.topLeft().x() > view->currentOffset().x())
        && (rc.topLeft().y() > view->currentOffset().y());
}

void BrowserView::myScrollTo(const QPoint &pt)
{
    page()->mainFrame()->setScrollPosition(pt);
    updateViewportRange();
}

QPointF BrowserView::currentOffset() const
{
    return page()->currentFrame()->scrollPosition();
}

void BrowserView::updateViewportRange()
{
    // Get current location.
    QSizeF s = page()->currentFrame()->contentsSize();
    QPointF pt = currentOffset();
    emit viewportUpdated(static_cast<int>(pt.y()),
                         static_cast<int>(rect().height()),
                         static_cast<int>(s.height()));
}

void BrowserView::formFocusedAddValue (const QString& form_id,
                                       const QString& form_name,
                                       const QString& form_action,
                                       const QString& input_type,
                                       const QString& input_id,
                                       const QString& input_name,
                                       const QString& value)
{
    QString scriptSource;

    if (!input_id.isEmpty())
    {
        scriptSource = "document.getElementById('" + input_id + "').value = '" + value + "';";
    }
    else if (!form_id.isEmpty())
    {
        scriptSource = 
            "var inputList = document.getElementById('" + form_id + "').getElementsByTagName('" + input_type + "');"
            "for (var i = 0; i < inputList.length; ++i) {"
                "if (inputList[i].name == '" + input_name + "') {"
                    "inputList[i].value = '" + value + "';"
                    "break;"
                "}"
            "}";
    }
    else if (!form_action.isEmpty())
    {
        scriptSource = 
            "var formList = document.getElementsByTagName('form');"
            "for (var i = 0; i < formList.length; ++i) {"
                "if (formList[i].action == '" + form_action + "') {"
                    "var inputList = formList[i].getElementsByTagName('" + input_type + "');"
                    "for (var j = 0; j < inputList.length; ++j) {"
                        "if (inputList[j].name == '" + input_name + "') {"
                            "inputList[j].value = '" + value + "';"
                            "break;"
                        "}"
                    "}"
                    "break;"
                "}"
            "}";
    }
    else if (!form_name.isEmpty())
    {
        scriptSource = 
            "var formList = document.getElementsByTagName('form');"
            "for (var i = 0; i < formList.length; ++i) {"
                "if (formList[i].name == '" + form_name + "') {"
                    "var inputList = formList[i].getElementsByTagName('" + input_type + "');"
                    "for (var j = 0; j < inputList.length; ++j) {"
                        "if (inputList[j].name == '" + input_name + "') {"
                            "inputList[j].value = '" + value + "';"
                            "break;"
                        "}"
                    "}"
                    "break;"
                "}"
            "}";
    }
    else
    {
        scriptSource = 
            "var inputList = document.getElementsByTagName('" + input_type + "');"
            "for (var i = 0; i < inputList.length; ++i) {"
                "if (inputList[i].name == '" + input_name + "') {"
                    "inputList[i].value = '" + value + "';"
                    "break;"
                "}"
            "}";
    }

    page()->mainFrame()->evaluateJavaScript(scriptSource);
}

void BrowserView::formFocused (const QString& form_id,
                               const QString& form_name,
                               const QString& form_action,
                               const QString& input_type,
                               const QString& input_id,
                               const QString& input_name)
{
    hand_tool_enabled_ = false;
    emit inputFormFocused(form_id, form_name, form_action, input_type, input_id, input_name);
}

void BrowserView::formLostFocus(void)
{
    hand_tool_enabled_ = true;
    emit inputFormLostFocus();
}

void BrowserView::populateJavaScriptWindowObject (void)
{
    page()->mainFrame()->addToJavaScriptWindowObject("__qWebViewWidget", this);
}

void BrowserView::addFormsFocusEvent(void)
{
    QString scriptSource =
        "function addFocusHandler(tagName) {"
            "var inputList = document.getElementsByTagName(tagName);"
            "for (var i = 0; i < inputList.length; ++i) {"
                "var formAction = inputList[i].form.action;"
                "var formName   = inputList[i].form.name;"
                "var formId     = inputList[i].form.id;"
                "var inputName  = inputList[i].name;"
                "var inputId    = inputList[i].id;"

                "if (tagName == 'input') {"
                    "var inputType = inputList[i].type;"
                    "if (inputType != 'password' && inputType != 'text')"
                        "continue;"
                "}"

                "inputList[i].setAttribute('onfocus', \"__qWebViewWidget.formFocused('\" + formId + \"', '\" + formName + \"', '\" + formAction + \"','\" + tagName + \"','\" + inputId + \"','\" + inputName + \"')\");"
                "inputList[i].setAttribute('onblur', '__qWebViewWidget.formLostFocus()');"
            "}"
        "}"
        "addFocusHandler('input');"
        "addFocusHandler('textarea');";

    page()->mainFrame()->evaluateJavaScript(scriptSource);
}

void BrowserView::addSelectEvents(void)
{
    QString scriptSource =
        "function addSelectHandler(tagName) {"
            "var selectList = document.getElementsByTagName(tagName);"
            "for (var i = 0; i < selectList.length; ++i) {"
                "selectList[i].setAttribute('onmouseup', '__qWebViewWidget.selectMouseUp()');"
                "selectList[i].setAttribute('onfocus', '__qWebViewWidget.selectFocus()');"
                "selectList[i].setAttribute('onblur', '__qWebViewWidget.selectBlur()');"
                "selectList[i].setAttribute('onchange', '__qWebViewWidget.selectChanged()');"
            "}"
        "}"
        "addSelectHandler('select');";

    page()->mainFrame()->evaluateJavaScript(scriptSource);
}

void BrowserView::setIsArticlePage(bool is_article)
{
    qDebug("Current page is article:%s", is_article ? "yes" : "no");
    is_current_page_readable_ = is_article;
    emit displayReaderButton(is_current_page_readable_);
}

void BrowserView::askForArticlePage()
{
    //QTimer::singleShot(0, this, SLOT(checkIsArticlePage()));
}

void BrowserView::checkIsArticlePage()
{
    QString getTitleSource =
        "function getTitleOfCurrentArticle(){"
             "var curTitle = '',"
             "origTitle = '';"
             "curTitle = origTitle = document.title;"
             "return curTitle;"
         "}";
    QString scriptSource = getTitleSource +
        "var article_title = getTitleOfCurrentArticle();"
        "if(!article_title) {"
            "__qWebViewWidget.setIsArticlePage(false);"
        "}"
        "else{"
            "__qWebViewWidget.setIsArticlePage(true);"
        "}";
    QString code = scriptSource;
    page()->mainFrame()->evaluateJavaScript(code);
}

void BrowserView::enterReaderMode(bool is_reader_mode)
{
    if (is_reader_mode)
    {
        QString code = readability_;// + "readability.init();";
        page()->mainFrame()->evaluateJavaScript(code);
    }
    else
    {
        reloadCurrentUrl();
    }
}

static void collectPopupMenus(const QObject *parent,
                              QWidgets &widgets)
{
    const QObjectList & all = parent->children();
    foreach(const QObject *object, all)
    {
         
        if (object->isWidgetType())
        {
            QComboBox * wnd = const_cast<QComboBox *>(static_cast<const QComboBox *>(object));
            if (wnd && wnd->isVisible())
            {
                widgets.push_back(wnd);
                collectPopupMenus(wnd, widgets);
            }
        }
    }
}

void BrowserView::selectMouseUp()
{
    QTimer::singleShot(0, this, SLOT(handleSelectPopup()));
}

void BrowserView::clearFocusWidgets()
{
    if (!focus_widgets_.isEmpty())
    {
        foreach(QWidget *widget, focus_widgets_)
        {
            widget->removeEventFilter(this);
        }
        focus_widgets_.clear();
    }
}

void BrowserView::handleSelectPopup()
{
    qDebug("handle select popup.");
    QWidget * focus_widget = QApplication::focusWidget();
    if (focus_widget != 0 && focus_widget != this)
    {
        // clear previous focused widgets
        clearFocusWidgets();

        // install event filters for new focused widgets
        collectPopupMenus(focus_widget, focus_widgets_);
        foreach(QWidget *widget, focus_widgets_)
        {
            widget->installEventFilter(this);
        }
    }
}

void BrowserView::selectFocus()
{
    qDebug("Select Focus");
    QTimer::singleShot(0, this, SLOT(handleSelectPopup()));
}

void BrowserView::selectBlur()
{
    clearFocusWidgets();
}

void BrowserView::selectChanged()
{
    qDebug("Select Changed");
}

bool BrowserView::eventFilter(QObject *obj, QEvent *e)
{
    qDebug("Select event:%d", e->type());
    return QObject::eventFilter(obj, e);
}

void BrowserView::focusOutEvent(QFocusEvent * e)
{
    qDebug("Focus Out Event");
    emit focusOut();
    QWebView::focusOutEvent(e);
}

void BrowserView::storeConf(const QUrl & url)
{
    if (url.isEmpty())
    {
        return;
    }

    QString path = url.path();
    QFileInfo info(path);
    if (!info.exists())
    {
        return;
    }
}

void BrowserView::clearHistory()
{
    this->history()->clear();
}

void BrowserView::displayBookmarks()
{
}

bool BrowserView::addBookmark()
{
    return false;
}

bool BrowserView::deleteBookmark()
{
    return false;
}

bool BrowserView::deletePassword()
{
    return false;
}

void BrowserView::clearCookies()
{
    WebApplication::accessManager()->clearCookies();
}

int BrowserView::GetHistoryPageCounts() const
{
    return history() ? history()->count() : -1;
}

int BrowserView::GetCurrentHistoryPageIndex() const
{
    return history() ? history()->currentItemIndex() : -1;
}

void BrowserView::onLoadProgress(int progress)
{
    if (progress_/10 != progress/10)
    {
        progress_ = progress;
        reportCurrentProcess();
    }
}

//bool BrowserView::focusNextPrevChild(bool next)
//{
    //bool focused = QWebView::focusNextPrevChild(next);
    //if (focused && isformFocused())
    //{
        //emit formFocused("", "", "", "", "", "");
    //}

    //return focused;
//}

bool BrowserView::isformFocused()
{
    foreach (QWebElement formElement, form_elements_)
    {
        if (!formElement.isNull() && formElement.hasFocus())
        {
            return true;
            break;
        }
    }

    return false;
}

QString BrowserView::getHtmlFromMainFrame()
{
    QWebPage* this_page = page();
    if (this_page != 0 && this_page->mainFrame() != 0)
    {
        return this_page->mainFrame()->toHtml();
    }
    return QString();
}

void BrowserView::onScaleBegin()
{
    qDebug("Web View begin scaling");
}

void BrowserView::onScaleEnd()
{
    qDebug("Web View end scaling");
}

void BrowserView::onScaling()
{
    qDebug("Web View is scaling");
}

}
