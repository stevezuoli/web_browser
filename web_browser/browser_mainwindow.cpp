#include <QtGui/QtGui>
#include <QScreen>
#include "browser_mainwindow.h"
#include "web_application.h"
#include "const_strings.h"
#include "Screen/screen_proxy.h"
#include "Screen/screen_update_watcher.h"
#include "common/WindowsMetrics.h"
#include "common/ImageManager.h"
#include "common/debug.h"
#include "url_lineedit.h"
#include "System/inc/system_manager.h"

using namespace ui::windowsmetrics;

namespace webbrowser
{
BrowserMainWindow::BrowserMainWindow(QWidget *parent)
#ifdef Q_WS_QWS
    : QWidget(parent, Qt::FramelessWindowHint)
#else
    : QWidget(parent)
#endif
    , history_back_tool_button_(this)
    , history_forward_tool_button_(this)
    , menu_tool_button_(this)
    , keyboard_button_(this)
    , address_lineedit_(new UrlLineEdit)
    , navigation_toolbar_(new DKToolBar(tr("Navigation"), this))
    , view_(new BrowserView)
    , keyboard_(new DKSoftKeyboardIME)
    , history_page_(new HistoryPage(view_, this))
    , menu_(this)
    , xiaomi_account_manager_()
    , evernote_account_manager_()
    , home_page_url_(ConstStrings::HOME_PAGE)
    , reader_mode_(false)
{
#ifndef Q_WS_QWS
    resize(GetWindowMetrics(UIScreenWidthIndex), GetWindowMetrics(UIScreenHeightIndex));
#else
    resize(qApp->desktop()->screenGeometry().size());
#endif
    setupToolBar();
    setupMenu();

    setAutoFillBackground(true);
    setBackgroundRole(QPalette::Base);

    address_lineedit_->setWebView(view_);

    history_page_->setViewer(view_);
    history_page_->setVisible(false);
    
    InitLayout();
    view_->setFocus();

    connect(view_, SIGNAL(linkClicked(const QUrl &)), this, SLOT(onLinkClicked(const QUrl &)));
    connect(view_, SIGNAL(urlChanged(const QUrl&)), this, SLOT(onUrlChanged(const QUrl&)));
    connect(view_, SIGNAL(loadFinished(bool)), this, SLOT(onLoadFinished(bool)));
    connect(view_, SIGNAL(keyboardKeyPressed(void)), this, SLOT(switchKeyboardVisible(void)));
    connect(view_, SIGNAL(inputFormFocused(const QString&, const QString&,
                                            const QString&, const QString&,
                                            const QString&, const QString&)),
            this, SLOT(onInputFormFocused(const QString&, const QString&,
                                          const QString&, const QString&,
                                          const QString&, const QString&)));
    connect(view_, SIGNAL(inputFormLostFocus()), this, SLOT(onInputFormLostFocus()));
    connect(view_, SIGNAL(displayTextOnAddressEdit(const QString&)), this, SLOT(onLineEditTextChanged(const QString&)));
    connect(view_, SIGNAL(scaleBegin()), this, SLOT(onViewScaleBegin()));
    connect(view_, SIGNAL(scaleEnd()), this, SLOT(onViewScaleEnd()));

    connect(history_page_, SIGNAL(historyPageQuit(const QString&)), this, SLOT(hideHistoryPage(const QString&)));
#ifdef Q_WS_QWS
    connect(qApp->desktop(), SIGNAL(resized(int)),
            this, SLOT(onScreenSizeChanged(int)), Qt::QueuedConnection);
#endif
}

BrowserMainWindow::~BrowserMainWindow()
{
}

static QUrl guessUrlFromString(const QString &string)
{
    DebugWB("\t%s", qPrintable(string));
    QString url_str = string.trimmed();
    QRegExp test(QLatin1String("^[a-zA-Z]+\\:.*"));

    // Check if it looks like a qualified URL. Try parsing it and see.
    bool has_schema = test.exactMatch(url_str);
    if (has_schema)
    {
        QUrl url = QUrl::fromEncoded(url_str.toUtf8(), QUrl::TolerantMode);
        if (url.isValid())
        {
            return url;
        }
    }

    // Might be a file.
    if (QFile::exists(url_str))
    {
        QFileInfo info(url_str);
        return QUrl::fromLocalFile(info.absoluteFilePath());
    }

    // Might be a shorturl - try to detect the schema.
    if (!has_schema)
    {
        int dot_index = url_str.indexOf(QLatin1Char('.'));
        if (dot_index != -1)
        {
            QString prefix = url_str.left(dot_index).toLower();
            QByteArray schema = (prefix == QLatin1String("ftp")) ? prefix.toLatin1() : "http";
            QUrl url = QUrl::fromEncoded(schema + "://" + url_str.toUtf8(), QUrl::TolerantMode);
            if (url.isValid())
            {
                return url;
            }
        }
    }

    // Fall back to QUrl's own tolerant parser.
    QUrl url = QUrl::fromEncoded(string.toUtf8(), QUrl::TolerantMode);

    // finally for cases where the user just types in a hostname add http
    if (url.scheme().isEmpty())
    {
        url = QUrl::fromEncoded("http://" + string.toUtf8(), QUrl::TolerantMode);
    }
    return url;
}

void BrowserMainWindow::load(const QString & url_str, const QString & option)
{
    // Depends on the url is local file or not, if the url is empty
    // we need to display the thumbnail view.
    // Check if it's a local file or not at first.
    qDebug("BrowserMainWindow::load %s, %s", qPrintable(url_str), qPrintable(option));
    QUrl url = guessUrlFromString(url_str);
    if (!url.isValid())
    {
        qDebug("Invalid input url");
        return;
    }
    else
    {
        if (XiaomiAccountManager::isXiaomiAccountPath(url_str))
        {
            xiaomi_account_manager_.reset(new XiaomiAccountManager);
            address_lineedit_->setModifyLineEditTextAutomatically(false);
            setupXiaomiAccountConnect();
            xiaomi_account_manager_->login(url_str, option);
        }
        else if (EvernoteAccountManager::isEvernotePath(url_str))
        {
            evernote_account_manager_.reset(new EvernoteAccountManager());
            setupEvernoteAccountConnection();
            evernote_account_manager_->login(url_str);
        }
        else
        {
            QString host = url.host();
            if (!host.isEmpty())
            {
                QString message = tr("%1");
                message = message.arg(host);
            }

            view_->myLoad(url);
        }
    }
}

bool BrowserMainWindow::event(QEvent *e)
{
    if (e->type() == QEvent::ToolTip ||
        e->type() == QEvent::HoverMove ||
        e->type() == QEvent::HoverEnter ||
        e->type() == QEvent::HoverLeave)
    {
        e->accept();
        return true;
    }

    bool ret = QWidget::event(e);
    return ret;
}

void BrowserMainWindow::setupToolBar()
{
    navigation_toolbar_->setStyleSheet("background: transparent;border: none");
    navigation_toolbar_->setFixedHeight(GetWindowMetrics(WebBrowserNavigationBarIconHeightIndex));
    navigation_toolbar_->setIconSize(QSize(GetWindowMetrics(WebBrowserNavigationBarIconWidthIndex), GetWindowMetrics(WebBrowserNavigationBarIconHeightIndex)));
    navigation_toolbar_->setFloatable(false);
    navigation_toolbar_->setMovable(false);
    address_lineedit_->setFixedHeight(GetWindowMetrics(WebBrowserAddressEditHeightIndex));
    navigation_toolbar_->addWidget(&history_back_tool_button_);
    navigation_toolbar_->addWidget(&history_forward_tool_button_);
    navigation_toolbar_->addWidget(address_lineedit_);
    navigation_toolbar_->addWidget(&keyboard_button_);
    navigation_toolbar_->addWidget(&menu_tool_button_);

    QString styleSheet("QToolButton::enabled{image: url(%1)}"
                       "QToolButton::pressed{image: url(%2)}" 
                       "QToolButton::disabled{image: url(%3)}");
    history_back_tool_button_.setStyleSheet(styleSheet.arg(
                ImageManager::GetImagePath(IMAGE_BACK), 
                ImageManager::GetImagePath(IMAGE_BACK_PRESSED),
                ImageManager::GetImagePath(IMAGE_BACK_GREY)).
                append("QToolButton{margin-left:%1px;}").
                arg(GetWindowMetrics(WebBrowserNavigationBarMarginIndex)));
    history_forward_tool_button_.setStyleSheet(styleSheet.arg(
                ImageManager::GetImagePath(IMAGE_FORWARD),
                ImageManager::GetImagePath(IMAGE_FORWARD_PRESSED),
                ImageManager::GetImagePath(IMAGE_FORWARD_GREY)));
    menu_tool_button_.setStyleSheet(styleSheet.arg(
                ImageManager::GetImagePath(IMAGE_MENU),
                ImageManager::GetImagePath(IMAGE_MENU_PRESSED),
                ImageManager::GetImagePath(IMAGE_MENU_DISABLED)).
                append("QToolButton{margin-right:%1px;}").
                arg(GetWindowMetrics(WebBrowserNavigationBarMarginIndex)));

    keyboard_button_.setStyleSheet(styleSheet.arg(
                ImageManager::GetImagePath(IMAGE_KEYBOARD),
                ImageManager::GetImagePath(IMAGE_KEYBOARD_PRESSED),
                ImageManager::GetImagePath(IMAGE_KEYBOARD_DISABLE)).
                append("QToolButton{margin-left:%1px;}").
                arg(GetWindowMetrics(UIPixelValue8Index)));

    updateBackForwardButtonStatus();
    connect(&history_back_tool_button_, SIGNAL(clicked()), this, SLOT(showBackHistoryPage()));
    connect(&history_forward_tool_button_, SIGNAL(clicked()), this, SLOT(showForwardHistoryPage()));
    connect(&menu_tool_button_, SIGNAL(clicked()), this, SLOT(showMenu()));
    connect(&keyboard_button_, SIGNAL(clicked()), this, SLOT(switchKeyboardVisible()));

    connect(address_lineedit_->lineEdit(), SIGNAL(returnPressed()), this, SLOT(openUrlInAddress()));
    connect(address_lineedit_->lineEdit(), SIGNAL(focusSignal(bool)), this, SLOT(onAddressInputFocus(bool)));
}

void BrowserMainWindow::keyReleaseEvent(QKeyEvent *ke)
{
    ke->accept();
    switch (ke->key())
    {
    case Qt::Key_Escape:
    case Qt::Key_Home:
        exitBrowser();
        break;
    case Qt::Key_Down:
        if (address_lineedit_->hasFocus())
        {
            view_->setFocus();
            return;
        }
    default:
        QWidget::keyReleaseEvent(ke);
        break;
    }
}

/// The keyPressEvent could be sent from virtual keyboard.
void BrowserMainWindow::keyPressEvent(QKeyEvent * ke)
{
    ke->accept();

    QKeyEvent * key_event = new QKeyEvent(ke->type(), ke->key(), ke->modifiers(), ke->text());
    QApplication::postEvent(view_, key_event);

    while (QApplication::hasPendingEvents())
    {
        QApplication::processEvents();
    }
}

void BrowserMainWindow::closeEvent(QCloseEvent *e)
{
    QWidget::closeEvent(e);
}

void BrowserMainWindow::onScreenSizeChanged(int)
{
    resize(qApp->desktop()->screenGeometry().size());
}

void BrowserMainWindow::onInputFormFocused(const QString& form_id,
                                      const QString& form_name,
                                      const QString& form_action,
                                      const QString& input_type,
                                      const QString& input_id,
                                      const QString& input_name)
{
    // fill keyboard private data
    //keyboard_priv_.form_action = form_action;
    //keyboard_priv_.form_id     = form_id;
    //keyboard_priv_.form_name   = form_name;
    //keyboard_priv_.input_type  = input_type;
    //keyboard_priv_.input_id    = input_id;
    //keyboard_priv_.input_name  = input_name;

    // update keyboard status
    //keyboard_status_ = FORM_FOCUSED;

    keyboard_->attachReceiver(view_);
    showSoftKeyboardIME(true);
}

void BrowserMainWindow::showHomePage()
{
    DebugWB("");
    showHistoryPage(false);
    load(home_page_url_);
}

void BrowserMainWindow::showBackHistoryPage()
{
    showHistoryPage(false);
    view_->back();
}

void BrowserMainWindow::showForwardHistoryPage()
{
    showHistoryPage(false);
    view_->forward();
}

void BrowserMainWindow::showMenu()
{
    showSoftKeyboardIME(false);
    QPoint popupPoint = navigation_toolbar_->rect().bottomRight();
    popupPoint.rx() -= menu_.sizeHint().width();
    menu_.popup(mapToGlobal(popupPoint));
}

void BrowserMainWindow::setupMenu()
{
    menu_actions[MA_BookStore] = menu_.addAction(tr("DuoKan bookstore"), this, SLOT(showHomePage()));
    menu_.addSeparator();
    //menu_.addAction(tr("Bookmark this page"), this, SLOT(bookmarkThisPage()))->setEnabled(false);
    //menu_.addSeparator();
    //menu_.addAction(tr("Bookmark manager"), this, SLOT(showBookmarkPage()))->setEnabled(false);
    //menu_.addSeparator();
    menu_actions[MA_History] = menu_.addAction(tr("History"), this, SLOT(showHistoryPage()));
    menu_.addSeparator();
    menu_actions[MA_Reader_Mode] = menu_.addAction(tr("Reader Mode"), this, SLOT(onReaderModeToggled()));
    menu_.addSeparator();
    menu_actions[MA_Zoom_In] = menu_.addAction(tr("ZoomIn"), this, SLOT(zoomIn()));
    menu_.addSeparator();
    menu_actions[MA_Zoom_Out] = menu_.addAction(tr("Zoomtout"), this, SLOT(zoomOut()));
    menu_.addSeparator();
    //menu_.addAction(tr("Settings"), this, SLOT(showSettingsPage()))->setEnabled(false);
    menu_actions[MA_Exit] = menu_.addAction(tr("Exit"), this, SLOT(exitBrowser()));
    //menu_.addSeparator();
    //menu_.addAction(tr("ScreentShot"), this, SLOT(printScreen()));
}

void BrowserMainWindow::updateMenuStatusInSpecialMode(bool inSpecialMode)
{
    for (int i = 0; i < MA_Count; ++i)
    {
        if (menu_actions[i])
        {
            menu_actions[i]->setVisible(inSpecialMode 
                    && (i == MA_Exit || i == MA_Zoom_In || i == MA_Zoom_Out));
        }
    }
}

void BrowserMainWindow::updateMenuStatusInHistoryPage(bool inHistoryPage)
{
    for (int i = 0; i < MA_Count; ++i)
    {
        if (menu_actions[i])
        {
            menu_actions[i]->setVisible(!inHistoryPage || (i == MA_Exit || i == MA_BookStore));
        }
    }
}

void BrowserMainWindow::showHistoryPage(bool show)
{
    if (history_page_->isVisible() == show)
    {
        return;
    }

    updateMenuStatusInHistoryPage(show);
    if (history_page_)
    {
        history_page_->setVisible(show);
        view_->setVisible(!show);
        if (show)
        {
            history_page_->setFocus();
        }
        else
        {
            view_->setFocus();
        }
    }
}
void BrowserMainWindow::hideHistoryPage(const QString& url)
{
    DebugWB("");
    showHistoryPage(false);
    if (!url.isEmpty())
        load(url);
}

void BrowserMainWindow::bookmarkThisPage()
{
}

void BrowserMainWindow::showBookmarkPage()
{
}

void BrowserMainWindow::showSettingsPage()
{
}

void BrowserMainWindow::onReaderModeToggled()
{
    reader_mode_ = !reader_mode_;
    menu_actions[MA_Reader_Mode]->setText(reader_mode_ ? tr("Normal Mode") : tr("Reader Mode"));
    view_->enterReaderMode(reader_mode_);
}

void BrowserMainWindow::updateBackForwardButtonStatus()
{
    history_back_tool_button_.setEnabled(view_->history()->canGoBack());
    history_forward_tool_button_.setEnabled(view_->history()->canGoForward());
}

void BrowserMainWindow::onUrlChanged(const QUrl& url)
{
    showHistoryPage(false);
    //if (!view_->isSpecialAccountMode())
    //{
        updateBackForwardButtonStatus();
    //}

    // reset reader mode button
    reader_mode_ = false;
    menu_actions[MA_Reader_Mode]->setText(tr("Reader Mode"));
}

void BrowserMainWindow::onLinkClicked(const QUrl &new_url)
{
    view_->myLoad(new_url);
}

void BrowserMainWindow::openUrlInAddress()
{
    DebugWB("\t%s", qPrintable(address_lineedit_->lineEdit()->text()));
    showHistoryPage(false);
    showSoftKeyboardIME(false);
    load(address_lineedit_->lineEdit()->text());
}

void BrowserMainWindow::showSoftKeyboardIME(bool show)
{
    if (keyboard_ != 0)
    {
        keyboard_->setParent(this);
        keyboard_->setVisible(show);
    }

    if (show)
        SystemManager::instance()->enterKeypadMode();
    else
        SystemManager::instance()->enterMouseMode();
}

void BrowserMainWindow::switchKeyboardVisible()
{
    if (keyboard_ != 0)
    {
        keyboard_->attachReceiver(view_);
        keyboard_->setParent(this);
        keyboard_->setVisible(!keyboard_->isVisible());
    }
}

void BrowserMainWindow::onInputFormLostFocus()
{
    keyboard_->attachReceiver(NULL);
    showSoftKeyboardIME(false);
    //keyboard_status_ = KEYBOARD_FREE;
}

void BrowserMainWindow::onAddressInputFocus(bool focusIn)
{
    keyboard_->attachReceiver(focusIn ? address_lineedit_->lineEdit() : NULL);
    showSoftKeyboardIME(focusIn);
}

void BrowserMainWindow::InitLayout()
{
    main_layout_.addWidget(navigation_toolbar_);
    main_layout_.addWidget(view_);
    main_layout_.addWidget(history_page_);
    main_layout_.addWidget(keyboard_);
    main_layout_.setSpacing(0);
    main_layout_.setContentsMargins(0, 0, 0, 0);
    setLayout(&main_layout_);
}

void BrowserMainWindow::onLoadFinished(bool ok)
{
    DebugWB("");
    if (view_->url().toString() == home_page_url_)
    {
        view_->history()->clear();
    }
    view_->setFocus();
    showSoftKeyboardIME(false);
}

void BrowserMainWindow::setupXiaomiAccountConnect()
{
    view_->setZoomFactor(xiaomi_account_manager_->getZoomFactor());
    view_->setSpecialAccountMode(true);
    updateMenuStatusInSpecialMode(true);
    address_lineedit_->setEnabled(false);

    connect(xiaomi_account_manager_.get(), SIGNAL(pageChanged(const QString&)), this, SLOT(onXiaomiAccountPageChanged(const QString&)));
    connect(xiaomi_account_manager_.get(), SIGNAL(loginFinished(bool)), this, SLOT(onXiaomiAccountLoadFinished(bool)));
    xiaomi_account_manager_->connectWebView(view_);
}
    
void BrowserMainWindow::setupEvernoteAccountConnection()
{
    connect(evernote_account_manager_.get(), SIGNAL(loginFinished(bool)), this, SLOT(onEvernoteAccountLoadFinished(bool)));
    evernote_account_manager_->connectWebView(view_);
}

void BrowserMainWindow::onXiaomiAccountPageChanged(const QString& message)
{
    onLineEditTextChanged(message);
}

void BrowserMainWindow::onLineEditTextChanged(const QString& message)
{
    address_lineedit_->lineEdit()->setText(message);
}

void BrowserMainWindow::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);
    QPainter painter(this);
    int lineTop = navigation_toolbar_->rect().bottom() - GetWindowMetrics(NavigationBarLinePixelIndex);
    painter.setBrush(Qt::black);
    painter.drawRect(0, lineTop, GetWindowMetrics(UIScreenWidthIndex), GetWindowMetrics(NavigationBarLinePixelIndex));
}

void BrowserMainWindow::exitBrowser()
{
    onLineEditTextChanged(tr("Exiting..."));
    QApplication::processEvents();
    view_->returnToLibrary();
}

void BrowserMainWindow::onXiaomiAccountLoadFinished(bool ok)
{
    if (ok)
    {
        xiaomi_account_manager_->disconnectWebView();
        exitBrowser();
    }
}
    
void BrowserMainWindow::onEvernoteAccountLoadFinished(bool ok)
{
    if (ok)
    {
        evernote_account_manager_->disconnectWebView();
        view_->returnToLibrary();
    }
}

static QVector<QRgb>& colorTable()
{
    static QVector<QRgb> color_table;
    if (color_table.size() <= 0)
    {
        for(int i = 0; i < 256; ++i)
        {
            color_table.push_back(qRgb(i, i, i));
        }
    }
    return color_table;
}

void BrowserMainWindow::printScreen()
{
    qDebug() << contentsMargins();
    QDateTime current_time = QDateTime::currentDateTime();
    qint64 current_time_num = current_time.toMSecsSinceEpoch();

#ifdef BUILD_FOR_ARM
    QString file_name = QString("/mnt/us/DK_ScreenShots/%1.png").arg(current_time_num);
#else
    QString file_name = QString("%1/%2.png").arg(QDir::homePath()).arg(current_time_num);
#endif
    QImage image(size(), QImage::Format_ARGB32);
    render(&image);
    QImage covert_image = image.convertToFormat(QImage::Format_Indexed8, colorTable());
    qDebug() << file_name;
    qDebug() << covert_image.save(file_name);
}

void BrowserMainWindow::zoomIn()
{
    view_->zoom(true);
    updateMenuStatusOnZoomFactorChanged();
}

void BrowserMainWindow::zoomOut()
{
    view_->zoom(false);
    updateMenuStatusOnZoomFactorChanged();
}

void BrowserMainWindow::onViewScaleBegin()
{
    showSoftKeyboardIME(false);
    menu_.setVisible(false);
}

void BrowserMainWindow::onViewScaleEnd()
{
    updateMenuStatusOnZoomFactorChanged();
}

void BrowserMainWindow::updateMenuStatusOnZoomFactorChanged()
{
    menu_actions[MA_Zoom_In]->setEnabled(!view_->isMaxiZoomConditionReached());
    menu_actions[MA_Zoom_Out]->setEnabled(!view_->isMiniZoomConditionReached());
}
}

