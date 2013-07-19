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
#include "Device/device.h"

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
    , title_label_(this)
    , address_lineedit_(new UrlLineEdit)
    , navigation_toolbar_(new DKToolBar(tr("Navigation"), this))
    , view_(new BrowserView)
    , keyboard_(new DKSoftKeyboardIME)
    , history_page_(new HistoryPage(view_, this))
    , menu_(this)
    , xiaomi_account_manager_()
    , evernote_account_manager_()
    , xiaomi_migration_manager_()
    , home_page_url_(ConstStrings::HOME_PAGE)
    , reader_mode_(false)
    , mouse_mode_(false)
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
    connect(view_, SIGNAL(keyboardKeyPressed(void)), this, SLOT(onKeyboardKeyPressed(void)));
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
    connect(view_, SIGNAL(mouseModeEnterd(bool)), this, SLOT(shiftMouseMode(bool)));
    connect(keyboard_, SIGNAL(keyboardKeyPressed(void)), this, SLOT(switchKeyboardVisibility(void)));
    connect(keyboard_, SIGNAL(keyboardVisibleChanged(bool)), this, SLOT(onKeyboardVisibleChanged(bool)));

    connect(history_page_, SIGNAL(historyPageQuit(const QUrl&)), this, SLOT(hideHistoryPage(const QUrl&)));
#ifdef Q_WS_QWS
    connect(qApp->desktop(), SIGNAL(resized(int)),
            this, SLOT(onScreenSizeChanged(int)), Qt::QueuedConnection);
#endif
    connect(&menu_, SIGNAL(becomeVisible(bool)), this, SLOT(shiftMouseMode(bool)));

    setCursor(QCursor(QPixmap(":/res/cursor.png")));

    view_->installEventFilter(this);
    history_page_->installEventFilter(this);
    menu_.installEventFilter(this);
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
        else if (XiaomiMigrationManager::isXiaomiMigrationPath(url_str))
        {
            xiaomi_migration_manager_.reset(new XiaomiMigrationManager(XiaomiMigrationManager::getHostFromPath(url_str)));
            setupXiaomiMigrationConnection();
            xiaomi_migration_manager_->start(option);
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

    toolbar_actions_[TBA_Back] = navigation_toolbar_->addWidget(&history_back_tool_button_);
    toolbar_actions_[TBA_Forward] = navigation_toolbar_->addWidget(&history_forward_tool_button_);
    toolbar_actions_[TBA_Edit] = navigation_toolbar_->addWidget(address_lineedit_);
    if (Device::isTouch())
    {
        toolbar_actions_[TBA_Keyboard] = navigation_toolbar_->addWidget(&keyboard_button_);
        toolbar_actions_[TBA_Menu] = navigation_toolbar_->addWidget(&menu_tool_button_);
    }
    else
    {
        title_label_.setContentsMargins(0, 10, 0, 10);
        title_label_.setText(tr("History"));
        title_label_.setFontSize(30);
        QSizePolicy sp = title_label_.sizePolicy();
        sp.setHorizontalPolicy(QSizePolicy::Expanding);
        title_label_.setSizePolicy(sp);
        title_label_.setAlignment(Qt::AlignCenter);
        toolbar_actions_[TBA_Title] = navigation_toolbar_->addWidget(&title_label_);
        toolbar_actions_[TBA_Title]->setVisible(false);

        //just for ui adjusted
        QLabel* label = new QLabel(navigation_toolbar_);
        label->setFixedWidth(GetWindowMetrics(UIHorizonMarginIndex));
        toolbar_actions_[TBA_Space] = navigation_toolbar_->addWidget(label);

        keyboard_button_.setVisible(false);
        menu_tool_button_.setVisible(false);
    }

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
    connect(&menu_tool_button_, SIGNAL(clicked()), this, SLOT(switchMenuVisibility()));
    connect(&keyboard_button_, SIGNAL(clicked()), this, SLOT(switchKeyboardVisibility()));

    connect(address_lineedit_, SIGNAL(returnPressed()), this, SLOT(openUrlInAddress()));
    connect(address_lineedit_, SIGNAL(focusSignal(bool)), this, SLOT(onAddressInputFocus(bool)));
}

bool BrowserMainWindow::eventFilter(QObject* watched, QEvent* event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_Escape && watched != &menu_ && watched != keyboard_)
        {
            onBackKeyPressed();
            DebugWB("eventfilter return true;");
            return true;
        }
    }

    return QWidget::eventFilter(watched, event);
}

void BrowserMainWindow::onBackKeyPressed()
{
    DebugWB("menu: %d, keyboard: %d", menu_.isVisible(), keyboard_->isVisible());
    if (history_page_->isVisible())
    {
        showHistoryPage(false);
    }
    else if (view_->hasFocus() && view_->isVisible())
    {
        showBackHistoryPage();
    }
}

void BrowserMainWindow::keyReleaseEvent(QKeyEvent *ke)
{
    ke->accept();
    switch (ke->key())
    {
    //case Qt::Key_Escape:
        //onBackKeyPressed();
        //break;
    case Qt::Key_Home:
        exitBrowser();
        break;
    case Qt::Key_Menu:
        switchMenuVisibility();
        break;
    case Qt::Key_Down:
        if (address_lineedit_->hasFocus())
        {
            if (view_->isVisible())
            {
                view_->setFocus();
            }
            else
            {
                history_page_->setFocus();
            }
        }
        else
        {
            QWidget::keyReleaseEvent(ke);
        }
        break;
    default:
        QWidget::keyReleaseEvent(ke);
        break;
    }
}

/// The keyPressEvent could be sent from virtual keyboard.
void BrowserMainWindow::keyPressEvent(QKeyEvent * ke)
{
    ke->ignore();
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
    DebugWB("");
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

void BrowserMainWindow::switchMenuVisibility()
{
    if (menu_.isVisible())
    {
        menu_.hide();
    }
    else
    {
        showSoftKeyboardIME(false);
        QPoint popupPoint = navigation_toolbar_->rect().bottomRight();
        popupPoint.rx() -= menu_.sizeHint().width();
        menu_.setActiveAction(menu_actions[MA_BookStore]);
        menu_.popup(mapToGlobal(popupPoint));
    }
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
    menu_actions[MA_Mouse] = menu_.addAction(tr("Mouse Mode"), this, SLOT(onMouseModeToggled()));
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
        if (show)
        {
            history_page_->setFocus();
        }
        else
        {
            view_->setFocus();
        }
        showOnlyTitleOnToolBar(show);
        history_page_->setVisible(show);
        view_->setVisible(!show);
    }
}
void BrowserMainWindow::hideHistoryPage(const QUrl& url)
{
    DebugWB("");
    showHistoryPage(false);
    if (!url.isEmpty())
    {
        view_->load(url);
    }
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

void BrowserMainWindow::onMouseModeToggled()
{
    mouse_mode_ = !mouse_mode_;
    menu_actions[MA_Mouse]->setText(mouse_mode_ ? tr("Keypad Mode") : tr("Mouse Mode"));
    qDebug("Mouse mode Toggled:%s", mouse_mode_ ? "true" : "false");
    shiftMouseMode(mouse_mode_);
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
    DebugWB("");
    view_->myLoad(new_url);
}

void BrowserMainWindow::openUrlInAddress()
{
    DebugWB("");
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
        SystemManager::instance()->setFastUpdate(show);
    }
}

void BrowserMainWindow::switchKeyboardVisibility()
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

    connect(xiaomi_account_manager_.get(), SIGNAL(pageChanged(const QString&)), this, SLOT(onAccountPageChanged(const QString&)));
    connect(xiaomi_account_manager_.get(), SIGNAL(loginFinished(bool)), this, SLOT(onXiaomiAccountLoadFinished(bool)));
    xiaomi_account_manager_->connectWebView(view_);
}
    
void BrowserMainWindow::setupXiaomiMigrationConnection()
{
    view_->setZoomFactor(xiaomi_migration_manager_->getZoomFactor());
    view_->setSpecialAccountMode(true);
    updateMenuStatusInSpecialMode(true);
    address_lineedit_->setEnabled(false);
    
    connect(xiaomi_migration_manager_.get(), SIGNAL(pageChanged(const QString&)), this, SLOT(onAccountPageChanged(const QString&)));
    connect(xiaomi_migration_manager_.get(), SIGNAL(migrationSucceeded()), this, SLOT(onXiaomiMigrationFinished()));
    connect(xiaomi_migration_manager_.get(), SIGNAL(migrationCanceled()), this, SLOT(onXiaomiMigrationFinished()));
    connect(xiaomi_migration_manager_.get(), SIGNAL(migrationFailed()), this, SLOT(onXiaomiMigrationFinished()));
    xiaomi_migration_manager_->connectWebView(view_);
}
    
void BrowserMainWindow::setupEvernoteAccountConnection()
{
    connect(evernote_account_manager_.get(), SIGNAL(loginFinished(bool)), this, SLOT(onEvernoteAccountLoadFinished(bool)));
    connect(evernote_account_manager_.get(), SIGNAL(pageChanged(const QString&)), this, SLOT(onAccountPageChanged(const QString&)));
    evernote_account_manager_->connectWebView(view_);
}

void BrowserMainWindow::onAccountPageChanged(const QString& message)
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
    
void BrowserMainWindow::onXiaomiMigrationFinished()
{
    xiaomi_migration_manager_->disconnectWebView();
    exitBrowser();
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

void BrowserMainWindow::showOnlyTitleOnToolBar(bool show)
{
    //qDebug() << __PRETTY_FUNCTION__ << " ########### " << show;
    toolbar_actions_[TBA_Back]->setVisible(!show);
    toolbar_actions_[TBA_Forward]->setVisible(!show);
    toolbar_actions_[TBA_Edit]->setVisible(!show);
    toolbar_actions_[TBA_Title]->setVisible(show);
}

void BrowserMainWindow::onKeyboardKeyPressed()
{
    if (!view_->isWebInputFocused())
    {
        address_lineedit_->lineEdit()->selectAll();
        address_lineedit_->setFocus();
        return;
    }
    if (address_lineedit_->lineEdit()->hasFocus())
    {
        address_lineedit_->lineEdit()->selectAll();
    }
    showSoftKeyboardIME(true);
}

void BrowserMainWindow::onKeyboardVisibleChanged(bool visible)
{
    if (!visible && view_ && view_->isVisible() && view_->hasFocus() && !menu_.isVisible())
    {
        shiftMouseMode(true);
    }
    else
    {
        shiftMouseMode(false);
    }
}

void BrowserMainWindow::shiftMouseMode(bool mouse)
{
    if (mouse_mode_)
    {
        qDebug("BrowserMainWindow::shiftMouseMode (%s)", mouse ? "true" : "false");
        if (mouse)
            SystemManager::instance()->enterMouseMode();
        else
            SystemManager::instance()->enterKeypadMode();
    }
    else if (!mouse)
    {
        SystemManager::instance()->enterKeypadMode();
    }
}

}

