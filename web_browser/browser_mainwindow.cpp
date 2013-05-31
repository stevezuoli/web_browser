#include <QtGui/QtGui>
#include "browser_mainwindow.h"
#include "web_application.h"
#include "const_strings.h"
#include "Screen/screen_proxy.h"
#include "Screen/screen_update_watcher.h"
#include "ui/DKSoftKeyboardIME.h"

namespace webbrowser
{
BrowserMainWindow::BrowserMainWindow(QWidget *parent)
#ifdef Q_WS_QWS
    : QMainWindow(parent, Qt::FramelessWindowHint)
#else
    : QMainWindow(parent)
#endif
    , homepage_action_(QIcon(QLatin1String(":/res/homepage@kt.png")), "", this)
    , history_back_action_(QIcon(QLatin1String(":/res/back@kt.png")), "", this)
    , history_forward_action_(QIcon(QLatin1String(":/res/forward@kt.png")), "", this)
    , menu_action_(QIcon(QLatin1String(":/res/menu@kt.png")), "", this)
    , navigation_toolbar_(tr("Navigation"), this)
    //, keyboard_status_(KEYBOARD_FREE)
    , m_homePageUrl(ConstStrings::HOME_PAGE)
{
    // setAttribute(Qt::WA_DeleteOnClose, true);
#ifndef Q_WS_QWS
    resize(600, 800);
#else
    resize(qApp->desktop()->screenGeometry().size());
#endif
    setupToolBar();

    setAutoFillBackground(true);
    setBackgroundRole(QPalette::Base);

    address_lineedit_.setWebView(&view_);
    
    setCentralWidget(&view_);

    connect(&homepage_action_, SIGNAL(triggered()), this, SLOT(showHomePage()));
    connect(&history_back_action_, SIGNAL(triggered()), this, SLOT(showBackHistoryPage()));
    connect(&history_forward_action_, SIGNAL(triggered()), this, SLOT(showForwardHistoryPage()));
    connect(&menu_action_, SIGNAL(triggered()), this, SLOT(showMenu()));
    connect(address_lineedit_.lineEdit(), SIGNAL(returnPressed()), this, SLOT(openUrlInAddress()));
    connect(address_lineedit_.lineEdit(), SIGNAL(focusSignal(bool)), this, SLOT(showSoftKeyboardIME(bool)));

    connect(&view_, SIGNAL(linkClicked(const QUrl &)), this, SLOT(onLinkClicked(const QUrl &)));
    connect(&view_, SIGNAL(urlChanged(const QUrl&)), this, SLOT(onUrlChanged(const QUrl&)));

    connect(&view_, SIGNAL(inputFormFocused(const QString&, const QString&,
                                            const QString&, const QString&,
                                            const QString&, const QString&)),
            this, SLOT(onInputFormFocused(const QString&, const QString&,
                                          const QString&, const QString&,
                                          const QString&, const QString&)));
    connect(&view_, SIGNAL(inputFormLostFocus()), this, SLOT(onInputFormLostFocus()));

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

void BrowserMainWindow::load(const QString & url_str)
{
    // Depends on the url is local file or not, if the url is empty
    // we need to display the thumbnail view.
    // Check if it's a local file or not at first.
    QUrl url = guessUrlFromString(url_str);
    if (!url.isValid())
    {
    }
    else
    {
        if (xiaomi_account_manager_.isXiaomiAccountPath(url_str))
        {
            xiaomi_account_manager_.connectWebView(&view_);
            xiaomi_account_manager_.login(); // test register
        }
        else
        {
            QString host = url.host();
            if (!host.isEmpty())
            {
                QString message = tr("%1");
                message = message.arg(host);
            }

            view_.myLoad(url);
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
    addToolBar(&navigation_toolbar_);
    navigation_toolbar_.setIconSize(QSize(40, 72));
    address_lineedit_.setFixedHeight(41);
    navigation_toolbar_.addAction(&homepage_action_);
    navigation_toolbar_.addAction(&history_back_action_);
    navigation_toolbar_.addAction(&history_forward_action_);
    navigation_toolbar_.addWidget(&address_lineedit_);
    navigation_toolbar_.addAction(&menu_action_);
}

void BrowserMainWindow::keyReleaseEvent(QKeyEvent *ke)
{
    ke->accept();
    switch (ke->key())
    {
    case Qt::Key_Escape:
        view_.returnToLibrary();
        break;
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
    QApplication::postEvent(&view_, key_event);

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
#ifndef WIN32
    qDebug("%s, %d, %d, %d", __PRETTY_FUNCTION__, view_.hasFocus(), address_lineedit_.hasFocus(), address_lineedit_.lineEdit()->hasFocus());
#endif
    // fill keyboard private data
    //keyboard_priv_.form_action = form_action;
    //keyboard_priv_.form_id     = form_id;
    //keyboard_priv_.form_name   = form_name;
    //keyboard_priv_.input_type  = input_type;
    //keyboard_priv_.input_id    = input_id;
    //keyboard_priv_.input_name  = input_name;

    // update keyboard status
    //keyboard_status_ = FORM_FOCUSED;

    DKSoftKeyboardIME::GetInstance()->attachReceiver(&view_);
    showSoftKeyboardIME(true);
}

void BrowserMainWindow::showHomePage()
{
    load(m_homePageUrl);
}

void BrowserMainWindow::showBackHistoryPage()
{
    view_.back();
}

void BrowserMainWindow::showForwardHistoryPage()
{
    view_.forward();
}

void BrowserMainWindow::showMenu()
{
}

void BrowserMainWindow::onUrlChanged(const QUrl& url)
{
    qDebug("BrowserMainWindow::onUrlChanged %s", qPrintable(url.toString()));
    int current = view_.GetCurrentHistoryPageIndex();
    int total = view_.GetHistoryPageCounts();
    if (current >= 0 && total > 0)
    {
        history_back_action_.setEnabled(current > 0);
        history_forward_action_.setEnabled(current != total -1);
    }

    address_lineedit_.lineEdit()->setText(url.toString());
}

void BrowserMainWindow::onLinkClicked(const QUrl &new_url)
{
    load(new_url.toString());
}

void BrowserMainWindow::openUrlInAddress()
{
    qDebug("BrowserMainWindow::openUrlInAddress, %s", address_lineedit_.lineEdit()->text().toStdString().c_str());
    load(address_lineedit_.lineEdit()->text());
}

void BrowserMainWindow::showSoftKeyboardIME(bool show)
{
    DKSoftKeyboardIME* ime = DKSoftKeyboardIME::GetInstance();
    if (ime)
    {
        ime->setParent(this);
        DKSoftKeyboardIME::GetInstance()->setVisible(show);
    }
}

void BrowserMainWindow::onInputFormLostFocus()
{
    DKSoftKeyboardIME::GetInstance()->attachReceiver(NULL);
    showSoftKeyboardIME(false);
    //keyboard_status_ = KEYBOARD_FREE;
}

void BrowserMainWindow::onAddressInputFocus()
{
    DKSoftKeyboardIME::GetInstance()->attachReceiver(address_lineedit_.lineEdit());
}
}

