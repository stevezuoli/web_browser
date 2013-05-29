#include "xiaomi_account_manager.h"
#include "access_manager.h"
#include "cookie_jar.h"

namespace network_service
{

#define TEST_SERVER

static const QString BOOK_HOST = "http://book.duokan.com";
static const QString LOGIN_HOST = "http://api.duokan.com";
static const QString TEST_HOST = "http://dkmars";

static const QString XIAOMI_CHECKIN = "/dk_id/api/checkin";
static const QString XIAOMI_EXCHANGE = "/dk_id/api/exchange";
static const QString XIAOMI_WEB_REGISTER = "/dk_id/api/xiaomi_web_reg";
static const QString XIAOMI_WEB_FOLLOWUP = "/dk_id/api/kindle_user_login";

static const QString MI_ACCOUNT_SERVICE_LOGIN_URI = "https://account.xiaomi.com/pass/serviceLogin";
static const QString MI_ACCOUNT_SERVICE_LOGIN_AUTH_URI = "https://account.xiaomi.com/pass/serviceLoginAuth";
static const QString MI_ACCOUNT_REGISTERED_CALLBACK_URI = "http://login.dushu.xiaomi.com/dk_id/api/checkin";

static char IntToHexChar(int i)
{
    if (0 <= i && i <= 9)
    {
        return i + '0';
    }
    else if (i < 16)
    {
        return i - 10 + 'a';
    }
    else
    {
        return 0;
    }
}

QString UrlEncode(const char* s)
{
    if (NULL == s)
    {
        return "";
    }
    const unsigned char* us = (const unsigned char*)s;
    QString result;
    while (unsigned int c = *us++)
    {
        if (isalnum(c))
        {
            result.push_back(c);
        }
        else if (' ' == c)
        {
            result.push_back('+');
        }
        else
        {
            result.push_back('%');
            result.push_back(IntToHexChar(c / 16));
            result.push_back(IntToHexChar(c % 16));
        }
    }
    return result;
}

class DuokanServerConfiguration
{
public:
    static bool isOnline()
    {
#ifdef TEST_SERVER
        return false;
#else
        return true;
#endif
    }

    static QString xiaomiCheckinUrl()
    {
#ifdef TEST_SERVER
        return TEST_HOST + XIAOMI_CHECKIN;
#else
        return LOGIN_HOST + XIAOMI_CHECKIN;
#endif
    }

    static QString xiaomiExchangeUrl()
    {
#ifdef TEST_SERVER
        return TEST_HOST + XIAOMI_EXCHANGE;
#else
        return LOGIN_HOST + XIAOMI_EXCHANGE;
#endif
    }

    static QString xiaomiWebRegisterUrl()
    {
#ifdef TEST_SERVER
        return (TEST_HOST + XIAOMI_WEB_REGISTER);
#else
        return (LOGIN_HOST + XIAOMI_WEB_REGISTER);
#endif

    }

    static QString xiaomiFollowupUrl()
    {
#ifdef TEST_SERVER
        QString str_url(TEST_HOST + XIAOMI_WEB_FOLLOWUP);
#else
        QString str_url(LOGIN_HOST + XIAOMI_WEB_FOLLOWUP);
#endif
        //QString transfered_url = UrlEncode(str_url.toStdString().c_str());
        return str_url;
    }

    static QString xiaomiAccountCallback()
    {
#ifdef TEST_SERVER
        return TEST_HOST + XIAOMI_CHECKIN;
#else
        return LOGIN_HOST + XIAOMI_CHECKIN;
#endif
    }
};

XiaomiAccountManager::XiaomiAccountManager()
    : view_(0)
{
}

XiaomiAccountManager::~XiaomiAccountManager()
{
}

QString XiaomiAccountManager::generateXiaomiAccountLoginUrl()
{
    return DuokanServerConfiguration::xiaomiWebRegisterUrl(); //+ "?followup=" + DuokanServerConfiguration::xiaomiFollowupUrl();
}

QString XiaomiAccountManager::getServiceTokenFromCookies(const QList<QNetworkCookie>& cookies)
{
    for (int i = cookies.count() - 1; i >= 0; --i)
    {
        qDebug("Cookie Name:%s, Value:%s", qPrintable(cookies.at(i).name()), qPrintable(cookies.at(i).value()));
        if (cookies.at(i).name().startsWith("serviceToken"))
        {
            return cookies.at(i).value();
        }
    }
    return QString();
}

void XiaomiAccountManager::onLoadStarted()
{
    QUrl current_url = view_->url();
    const QString & myUrl = current_url.toString();
    if (myUrl.startsWith(DuokanServerConfiguration::xiaomiFollowupUrl()))
    {
        view_->stop();
        QList<QNetworkCookie> cookies = getAccessManagerInstance()->cookieJar()->cookiesForUrl(current_url);
        QString serviceToken = getServiceTokenFromCookies(cookies);
        if (!serviceToken.isEmpty())
        {
            exchangeDuokanToken(serviceToken);
        }
    }
    else
    {
        if (myUrl.startsWith(DuokanServerConfiguration::xiaomiWebRegisterUrl()))
        {
            emit startLogin();
        }
        else if (myUrl.startsWith(MI_ACCOUNT_SERVICE_LOGIN_AUTH_URI))
        {
            emit startLoginAuth();
        }
    }
}

void XiaomiAccountManager::onLoadFinished(bool ok)
{
    QUrl current_url = view_->url();
    QString myUrl = current_url.toEncoded();
    if (myUrl.startsWith(MI_ACCOUNT_SERVICE_LOGIN_URI))
    {
        emit loginFinished(ok);
    }
    else if (myUrl.contains(DuokanServerConfiguration::xiaomiAccountCallback()))
    {
        qDebug("test");
    }
    else if (myUrl.startsWith(MI_ACCOUNT_REGISTERED_CALLBACK_URI) &&
        !DuokanServerConfiguration::isOnline())
    {
        CookieJar* cookieJar = dynamic_cast<CookieJar*>(getAccessManagerInstance()->cookieJar());
        cookieJar->clear();
        QString targetUrl = myUrl.replace(MI_ACCOUNT_REGISTERED_CALLBACK_URI,
            DuokanServerConfiguration::xiaomiAccountCallback());
        load(targetUrl);
    }
}

void XiaomiAccountManager::onUrlChanged(const QUrl& url)
{
    QString myUrl = url.toString();
    if (myUrl.startsWith(MI_ACCOUNT_REGISTERED_CALLBACK_URI) &&
        !DuokanServerConfiguration::isOnline())
    {
        //view_->stop();
    }
    else if (myUrl.startsWith(DuokanServerConfiguration::xiaomiFollowupUrl()))
    {
        QList<QNetworkCookie> cookies = getAccessManagerInstance()->cookieJar()->cookiesForUrl(url);
        QString serviceToken = getServiceTokenFromCookies(cookies);
        exchangeDuokanToken(serviceToken);
    }
}

bool XiaomiAccountManager::isXiaomiAccountPath(const QString& path)
{
    return path.contains(DuokanServerConfiguration::xiaomiWebRegisterUrl());
}

void XiaomiAccountManager::connectWebView(QWebView* view)
{
    view_ = view;

    // Setup connections.
    connect(view_, SIGNAL(loadStarted()), this, SLOT(onLoadStarted()));
    connect(view_, SIGNAL(loadFinished(bool)), this, SLOT(onLoadFinished(bool)));
    connect(view_, SIGNAL(urlChanged(const QUrl&)), this, SLOT(onUrlChanged(const QUrl&)));
}

void XiaomiAccountManager::disconnectWebView()
{
    if (view_ != 0)
    {
        // Setup connections.
        disconnect(view_, SIGNAL(loadStarted()), this, SLOT(onLoadStarted()));
        disconnect(view_, SIGNAL(loadFinished(bool)), this, SLOT(onLoadFinished(bool)));
        disconnect(view_, SIGNAL(urlChanged(const QUrl&)), this, SLOT(onUrlChanged(const QUrl&)));

        view_ = 0;
    }
}

bool XiaomiAccountManager::exchangeDuokanToken(const QString& serviceToken)
{
    // TODO. Parse Xiaomi ID from service Token.
    return false;
}

void XiaomiAccountManager::load(const QString& path)
{
    QUrl my_url = guessUrlFromString(path);
    if (view_ != 0 && my_url.isValid())
    {
        view_->load(my_url);
    }
}

void XiaomiAccountManager::login()
{
    QString login_str = generateXiaomiAccountLoginUrl();
    QUrl url = guessUrlFromString(login_str);
    url.addEncodedQueryItem("followup", DuokanServerConfiguration::xiaomiFollowupUrl().toUtf8());

    QByteArray str = url.toEncoded();
    view_->load(url);
}

}
