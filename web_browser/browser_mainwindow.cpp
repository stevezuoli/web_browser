#include <QtGui/QtGui>
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
    , exit_tool_button_(this)
    , history_back_tool_button_(this)
    , history_forward_tool_button_(this)
    , menu_tool_button_(this)
    , address_lineedit_(new UrlLineEdit)
    , navigation_toolbar_(new DKToolBar(tr("Navigation"), this))
    , view_(new BrowserView)
    , keyboard_(new DKSoftKeyboardIME)
    , xiaomi_account_manager_()
    , m_homePageUrl(ConstStrings::HOME_PAGE)
{
    // setAttribute(Qt::WA_DeleteOnClose, true);
#ifndef Q_WS_QWS
    resize(GetWindowMetrics(UIScreenWidthIndex), GetWindowMetrics(UIScreenHeightIndex));
#else
    resize(qApp->desktop()->screenGeometry().size());
#endif
    setupToolBar();

    setAutoFillBackground(true);
    setBackgroundRole(QPalette::Base);

    address_lineedit_->setWebView(view_);
    
    InitLayout();
    view_->setFocus();
    connect(&exit_tool_button_, SIGNAL(clicked()), view_, SLOT(returnToLibrary()));
    connect(&history_back_tool_button_, SIGNAL(clicked()), this, SLOT(showBackHistoryPage()));
    connect(&history_forward_tool_button_, SIGNAL(clicked()), this, SLOT(showForwardHistoryPage()));
    connect(&menu_tool_button_, SIGNAL(clicked()), this, SLOT(showMenu()));
    connect(address_lineedit_->lineEdit(), SIGNAL(returnPressed()), this, SLOT(openUrlInAddress()));
    connect(address_lineedit_->lineEdit(), SIGNAL(focusSignal(bool)), this, SLOT(onAddressInputFocus(bool)));
    connect(view_, SIGNAL(displayReaderButton(bool)), address_lineedit_, SLOT(displayReaderButton(bool)));
    connect(address_lineedit_, SIGNAL(enterReaderMode(bool)), view_, SLOT(enterReaderMode(bool)));

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

    
#ifdef Q_WS_QWS
    connect(qApp->desktop(), SIGNAL(resized(int)),
            this, SLOT(onScreenSizeChanged(int)), Qt::QueuedConnection);
#endif
}

BrowserMainWindow::~BrowserMainWindow()
{
    DebugWB("");
}

static QUrl guessUrlFromString(const QString &string)
{
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
            if (option.compare("login", Qt::CaseInsensitive) == 0)
                xiaomi_account_manager_->login(url_str, true); // test register
            else if (option.compare("register", Qt::CaseInsensitive) == 0)
                xiaomi_account_manager_->login(url_str, false);
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
    navigation_toolbar_->setIconSize(QSize(GetWindowMetrics(WebBrowserNavigationBarIconWidthIndex), GetWindowMetrics(WebBrowserNavigationBarIconHeightIndex)));
    navigation_toolbar_->setFloatable(false);
    navigation_toolbar_->setMovable(false);
    address_lineedit_->setFixedHeight(GetWindowMetrics(WebBrowserAddressEditHeightIndex));
    navigation_toolbar_->addWidget(&exit_tool_button_);
    navigation_toolbar_->addWidget(&history_back_tool_button_);
    navigation_toolbar_->addWidget(&history_forward_tool_button_);
    navigation_toolbar_->addWidget(address_lineedit_);
    navigation_toolbar_->addWidget(&menu_tool_button_);

    QString styleSheet("QToolButton::enabled{image: url(%1)}" "QToolButton::pressed{image: url(%2)}" "QToolButton::disabled{image: url(%3)}");
    //QString styleSheetDisabled("disabled{image: url(%1)}");
    exit_tool_button_.setStyleSheet(styleSheet.arg(
                ImageManager::GetImagePath(IMAGE_HOMEPAGE),
                ImageManager::GetImagePath(IMAGE_BROWSER),
                ImageManager::GetImagePath(IMAGE_HOMEPAGE)));
    history_back_tool_button_.setStyleSheet(styleSheet.arg(
                ImageManager::GetImagePath(IMAGE_BACK), 
                ImageManager::GetImagePath(IMAGE_BROWSER),
                ImageManager::GetImagePath(IMAGE_BACK_GREY)));
    history_forward_tool_button_.setStyleSheet(styleSheet.arg(
                ImageManager::GetImagePath(IMAGE_FORWARD),
                ImageManager::GetImagePath(IMAGE_BROWSER),
                ImageManager::GetImagePath(IMAGE_FORWARD_GREY)));
    menu_tool_button_.setStyleSheet(styleSheet.arg(
                ImageManager::GetImagePath(IMAGE_MENU),
                ImageManager::GetImagePath(IMAGE_BROWSER),
                ImageManager::GetImagePath(IMAGE_MENU)));
    //history_back_tool_button_.setIcon(QIcon(ImageManager::GetImagePath(IMAGE_BACK)));
    //history_forward_tool_button_.setIcon(QIcon(ImageManager::GetImagePath(IMAGE_FORWARD)));
    //menu_tool_button_.setIcon(QIcon(ImageManager::GetImagePath(IMAGE_MENU)));
}

void BrowserMainWindow::keyReleaseEvent(QKeyEvent *ke)
{
    ke->accept();
    switch (ke->key())
    {
    case Qt::Key_Escape:
        view_->returnToLibrary();
        break;
    default:
        QWidget::keyReleaseEvent(ke);
        break;
    }
}

/// The keyPressEvent could be sent from virtual keyboard.
void BrowserMainWindow::keyPressEvent(QKeyEvent * ke)
{
    DebugWB("");
    ke->accept();

    switch (ke->key())
    {
    case Qt::Key_Down:
        if (address_lineedit_->hasFocus())
        {
            view_->setFocus();
            return;
        }
    default:
        break;
    }

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
    //DebugWB("");
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
    load(m_homePageUrl);
}

void BrowserMainWindow::showBackHistoryPage()
{
    view_->back();
}

void BrowserMainWindow::showForwardHistoryPage()
{
    view_->forward();
}

void BrowserMainWindow::showMenu()
{
}

void BrowserMainWindow::onUrlChanged(const QUrl& url)
{
    qDebug("BrowserMainWindow::onUrlChanged %s", qPrintable(url.toString()));
    if (!xiaomi_account_manager_)
    {
        int current = view_->GetCurrentHistoryPageIndex();
        int total = view_->GetHistoryPageCounts();
        if (current >= 0 && total > 0)
        {
            history_back_tool_button_.setEnabled(current > 0);
            history_forward_tool_button_.setEnabled(current != total -1);
        }
    }
}

void BrowserMainWindow::onLinkClicked(const QUrl &new_url)
{
    view_->myLoad(new_url);
}

void BrowserMainWindow::openUrlInAddress()
{
    showSoftKeyboardIME(false);
    view_->setFocus();
    load(address_lineedit_->lineEdit()->text());
}

void BrowserMainWindow::showSoftKeyboardIME(bool show)
{
    //DebugWB("");
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
        keyboard_->setParent(this);
        keyboard_->setVisible(!keyboard_->isVisible());
    }
}

void BrowserMainWindow::onInputFormLostFocus()
{
    //DebugWB("");
    keyboard_->attachReceiver(NULL);
    showSoftKeyboardIME(false);
    //keyboard_status_ = KEYBOARD_FREE;
}

void BrowserMainWindow::onAddressInputFocus(bool focusIn)
{
    DebugWB("");
    keyboard_->attachReceiver(focusIn ? address_lineedit_->lineEdit() : NULL);
    showSoftKeyboardIME(focusIn);
}

void BrowserMainWindow::InitLayout()
{
    main_layout_.addWidget(navigation_toolbar_);
    main_layout_.addWidget(view_);
    main_layout_.addWidget(keyboard_);
    setLayout(&main_layout_);
}

void BrowserMainWindow::onLoadFinished(bool ok)
{
    DebugWB("");
    view_->setFocus();
    showSoftKeyboardIME(false);
}

void BrowserMainWindow::setupXiaomiAccountConnect()
{
    connect(xiaomi_account_manager_.get(), SIGNAL(pageChanged(const QString&)), this, SLOT(onXiaomiAccountPageChanged(const QString&)));
    connect(xiaomi_account_manager_.get(), SIGNAL(loginFinished(bool)), this, SLOT(onXiaomiAccountLoadFinished(bool)));
    xiaomi_account_manager_->connectWebView(view_);
}

void BrowserMainWindow::onXiaomiAccountPageChanged(const QString& message)
{
    address_lineedit_->lineEdit()->setText(message);
}

void BrowserMainWindow::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    int lineTop = navigation_toolbar_->rect().bottom();
    painter.setBrush(Qt::black);
    painter.drawRect(0, lineTop, GetWindowMetrics(UIScreenWidthIndex), GetWindowMetrics(NavigationBarLinePixelIndex));
    QWidget::paintEvent(e);
}

void BrowserMainWindow::onXiaomiAccountLoadFinished(bool ok)
{
    if (ok)
    {
        xiaomi_account_manager_->disconnectWebView();
        view_->returnToLibrary();
    }
}

}

