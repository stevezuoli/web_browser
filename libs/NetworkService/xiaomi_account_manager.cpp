#include "xiaomi_account_manager.h"
#include "access_manager.h"
#include "cookie_jar.h"

namespace network_service
{

static const QString BOOK_HOST = "http://book.duokan.com";
static const QString LOGIN_HOST = "http://api.duokan.com";
static const QString TEST_HOST = "dkmars";

static const QString XIAOMI_CHECKIN = "/dk_id/api/checkin";
static const QString XIAOMI_EXCHANGE = "/dk_id/api/exchange";
static const QString XIAOMI_WEB_REGISTER = "/dk_id/api/xiaomi_web_reg";
static const QString XIAOMI_WEB_FOLLOWUP = "/dk_id/api/followup";

static const QString MI_ACCOUNT_SERVICE_LOGIN_URI = "https://account.xiaomi.com/pass/serviceLogin";
static const QString MI_ACCOUNT_SERVICE_LOGIN_AUTH_URI = "https://account.xiaomi.com/pass/serviceLoginAuth";
static const QString MI_ACCOUNT_REGISTERED_CALLBACK_URI = "http://login.dushu.xiaomi.com/dk_id/api/checkin";

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
        return TEST_HOST + XIAOMI_WEB_REGISTER;
#else
        return LOGIN_HOST + XIAOMI_WEB_REGISTER;
#endif
    }

    static QString xiaomiFollowupUrl()
    {
#ifdef TEST_SERVER
        return TEST_HOST + XIAOMI_WEB_FOLLOWUP;
#else
        return LOGIN_HOST + XIAOMI_WEB_FOLLOWUP;
#endif
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

XiaomiAccountManager::XiaomiAccountManager(QWebView* view)
    : view_(view)
{
    // Setup connections.
    connect(view_, SIGNAL(loadStarted()), this, SLOT(onLoadStarted()));
    connect(view_, SIGNAL(loadFinished(bool)), this, SLOT(onLoadFinished(bool)));
    connect(view_, SIGNAL(urlChanged(const QUrl&)), this, SLOT(onUrlChanged(const QUrl&)));
}

XiaomiAccountManager::~XiaomiAccountManager()
{
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
    const QString & myUrl = current_url.toString();
    if (myUrl.startsWith(MI_ACCOUNT_SERVICE_LOGIN_URI))
    {
        emit loginFinished(ok);
    }
}

void XiaomiAccountManager::onUrlChanged(const QUrl& url)
{
    QUrl current_url = view_->url();
    QString & myUrl = current_url.toString();
    if (myUrl.startsWith(MI_ACCOUNT_REGISTERED_CALLBACK_URI) &&
        !DuokanServerConfiguration::isOnline())
    {
        CookieJar* cookieJar = dynamic_cast<CookieJar*>(getAccessManagerInstance()->cookieJar());
        cookieJar->clear();
        QString targetUrl = myUrl.replace(MI_ACCOUNT_REGISTERED_CALLBACK_URI,
            DuokanServerConfiguration::xiaomiAccountCallback());
        view_->load(QUrl(targetUrl));
    }
    else if (myUrl.startsWith(DuokanServerConfiguration::xiaomiFollowupUrl()))
    {
        QList<QNetworkCookie> cookies = getAccessManagerInstance()->cookieJar()->cookiesForUrl(current_url);
        QString serviceToken = getServiceTokenFromCookies(cookies);
        exchangeDuokanToken(serviceToken);
    }
}

bool XiaomiAccountManager::exchangeDuokanToken(const QString& serviceToken)
{
    // TODO. Parse Xiaomi ID from service Token.
    return false;
}

}