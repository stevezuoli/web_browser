#include <QUrl>
#include "ns_utils.h"
#include "cookie_jar.h"
#include "access_manager.h"
#include "evernote_account_manager.h"
#include "Database/token_ring.h"

using namespace web_database;

namespace network_service
{

static const QString EVERNOTE_HOST_SANDBOX = "https://sandbox.evernote.com";
static const QString EVERNOTE_HOST_PRODUCT = "https://www.evernote.com";
static const QString EVERNOTE_HOST_YINXIANG = "https://app.yinxiang.com";
static const QString CALLBACK_URI = "http://kindle.duokan.com/evernote_callback";
static const QString CONSUMER_KEY = "pkunetspy-2850";
static const QString CONSUMER_SECRET = "4893c76543f56bdd";

class EvernoteUrlManager
{
    EvernoteUrlManager();
public:
    ~EvernoteUrlManager();
    static QUrl getRequestEndPoint(const QString& option);
    static QString getAuthorizationUri(const QString& option, const QString& request_token);
    static QString getCallbackUrlString();
    static QUrl getCallbackUrl();
    static QString getHostString(const QString& option);
    static QString getConsumerKey();
    static QString getConsumerSecret();
};

QString EvernoteUrlManager::getHostString(const QString& option)
{
    QString host = EVERNOTE_HOST_PRODUCT;
    if (option.startsWith("sandbox.evernote.com"))
    {
        host = EVERNOTE_HOST_SANDBOX;
    }
    else if (option.startsWith("www.evernote.com"))
    {
        host = EVERNOTE_HOST_PRODUCT;
    }
    else if (option.startsWith("app.yinxiang.com"))
    {
        host = EVERNOTE_HOST_YINXIANG;
    }
    return host;
}

QUrl EvernoteUrlManager::getRequestEndPoint(const QString& option)
{
    QString host = getHostString(option);
    QString url_str = host + "/oauth";
    return guessUrlFromString(url_str);
}

QString EvernoteUrlManager::getAuthorizationUri(const QString& option, const QString& request_token)
{
    return getHostString(option) + "/OAuth.action?oauth_token=" + request_token;
}

QUrl EvernoteUrlManager::getCallbackUrl()
{
    return guessUrlFromString(CALLBACK_URI);
}

QString EvernoteUrlManager::getCallbackUrlString()
{
    return CALLBACK_URI;
}

QString EvernoteUrlManager::getConsumerKey()
{
    return CONSUMER_KEY;
}

QString EvernoteUrlManager::getConsumerSecret()
{
    return CONSUMER_SECRET;
}

EvernoteAccountManager::EvernoteAccountManager()
    : view_(0)
    , oauth_manager_(new KQOAuthManager(this))
    , oauth_request_(new KQOAuthRequest(this))
{
    connect(this, SIGNAL(askForAuthorization(QString)), SLOT(onAuthorization(QString)));
}
    
EvernoteAccountManager::~EvernoteAccountManager()
{
}

bool EvernoteAccountManager::isEvernotePath(const QString& path)
{
    return (path.compare("www.evernote.com", Qt::CaseInsensitive) == 0 ||
            path.compare("app.yinxiang.com", Qt::CaseInsensitive) == 0 ||
            path.compare("sandbox.evernote.com", Qt::CaseInsensitive) == 0 );
}

void EvernoteAccountManager::login(const QString& option)
{
    emit pageChanged(tr("Direct to Evernote..."));
    askForRequestToken(option);
}

void EvernoteAccountManager::askForRequestToken(const QString& option)
{
    user_option_ = option;
    QUrl request_endpoint = EvernoteUrlManager::getRequestEndPoint(option);
    oauth_request_->initRequest(KQOAuthRequest::TemporaryCredentials, request_endpoint);
    oauth_request_->setConsumerKey(EvernoteUrlManager::getConsumerKey());
    oauth_request_->setConsumerSecretKey(EvernoteUrlManager::getConsumerSecret());
    oauth_request_->setCallbackUrl(EvernoteUrlManager::getCallbackUrl());

    connect(oauth_manager_.get(), SIGNAL(requestReady(QByteArray)), this, SLOT(onRequestReady(QByteArray)));
    oauth_manager_->executeRequest(oauth_request_.get());
}

void EvernoteAccountManager::onRequestReady(QByteArray reply)
{
    qDebug("Request Token Result:%d", oauth_manager_->lastError());
    if (oauth_manager_->lastError() == KQOAuthManager::NoError)
    {
        parseRequestTokenAndSecret(reply);
        emit askForAuthorization(EvernoteUrlManager::getAuthorizationUri(user_option_, request_token_));
    }
    disconnect(oauth_manager_.get(), SIGNAL(requestReady(QByteArray)), this, SLOT(onRequestReady(QByteArray)));
}

void EvernoteAccountManager::onAuthorization(QString auth_url)
{
    if (!auth_url.isEmpty())
    {
        QUrl my_url = guessUrlFromString(auth_url);
        if (view_ != 0 && my_url.isValid())
        {
            view_->load(my_url);
        }
    }
}

void EvernoteAccountManager::onUrlChanged(const QUrl& url)
{
    QString my_url = url.toEncoded();
    if (my_url.startsWith(EvernoteUrlManager::getCallbackUrlString()))
    {
        view_->stop();
        if (parseVerifier(my_url))
        {
            askForAccessToken();
        }
    }
}

static QMap<QString, QString> parseUrl(const QString& url_str)
{
    QStringList raw_list = url_str.split('&');
    QMap<QString, QString> splitted;
    foreach(QString raw_str, raw_list)
    {
        QStringList sub_split_list = raw_str.split('=');
        if (sub_split_list.size() > 1)
        {
            splitted[sub_split_list[0]] = sub_split_list[1];
        }
    }
    return splitted;
}

bool EvernoteAccountManager::parseVerifier(const QString& url_str)
{
    QMap<QString, QString> splitted = parseUrl(url_str);
    if(!splitted.contains("oauth_verifier"))
    {
        return false;
    }
    verifier_ = splitted["oauth_verifier"];
    return true;
}

bool EvernoteAccountManager::parseRequestTokenAndSecret(const QByteArray& reply)
{
    request_token_ = reply.split('&')[0].split('=')[1];
    request_token_secret_ = reply.split('&')[1].split('=')[1];
    return true;
}
    
void EvernoteAccountManager::askForAccessToken()
{
    QUrl request_endpoint = EvernoteUrlManager::getRequestEndPoint(user_option_);
    oauth_request_->initRequest(KQOAuthRequest::AccessToken, request_endpoint);
    oauth_request_->setConsumerKey(EvernoteUrlManager::getConsumerKey());
    oauth_request_->setConsumerSecretKey(EvernoteUrlManager::getConsumerSecret());
    oauth_request_->setToken(request_token_);
    oauth_request_->setTokenSecret(request_token_secret_);
    oauth_request_->setVerifier(verifier_);

    if (oauth_request_->isValid())
    {
        connect(oauth_manager_.get(), SIGNAL(requestReady(QByteArray)), this, SLOT(onAccessTokenReady(QByteArray)));
        oauth_manager_->executeRequest(oauth_request_.get());
    }
}

void EvernoteAccountManager::onAccessTokenReady(QByteArray reply)
{
    // TODO. use json to parse it?
    EvernoteToken token(user_option_, reply);
    TokenRing token_ring;
    token_ring.setEvernoteToken(token);
    token.save();
    emit loginFinished(true);
}

void EvernoteAccountManager::connectWebView(QWebView* view)
{
    view_ = view;
    connect(view_, SIGNAL(urlChanged(const QUrl&)), this, SLOT(onUrlChanged(const QUrl&)));

    // Use customized cookie jar
    CookieJar* xiaomi_cookie_jar = dynamic_cast<CookieJar*>(getAccessManagerInstance()->cookieJar());
    xiaomi_cookie_jar->setStorageName(QLatin1String("/evernote_cookies.ini"));
    xiaomi_cookie_jar->setSettingName(QLatin1String("evernote_cookies"));
    xiaomi_cookie_jar->load();
    if (xiaomi_cookie_jar->keepPolicy() != CookieJar::KeepUntilExit)
    {
        xiaomi_cookie_jar->setKeepPolicy(CookieJar::KeepUntilExit);
    }
}

void EvernoteAccountManager::disconnectWebView()
{
    view_ = 0;

    // Use default cookie jar
    CookieJar* xiaomi_cookie_jar = dynamic_cast<CookieJar*>(getAccessManagerInstance()->cookieJar());
    xiaomi_cookie_jar->reset();
}

}
