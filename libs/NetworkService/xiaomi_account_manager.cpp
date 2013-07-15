#include "xiaomi_account_manager.h"
#include "access_manager.h"
#include "cookie_jar.h"
#include "ns_utils.h"

#include "Device/DeviceInfo.h"
#include "Device/fat.h"
#include "AES/aes.h"
#include "Database/token_ring.h"

#define TEST_SERVER

using namespace web_database;

namespace network_service
{

static const QString BOOK_HOST = "http://book.duokan.com";
static const QString LOGIN_HOST = "http://login.dushu.xiaomi.com";
static const QString TEST_HOST = "http://dkmars";

static const QString XIAOMI_CHECKIN = "/dk_id/api/checkin";
static const QString XIAOMI_EXCHANGE = "/dk_id/api/exchange";
static const QString XIAOMI_WEB_REGISTER = "/dk_id/api/xiaomi_web_reg";
static const QString XIAOMI_WEB_FOLLOWUP = "/dk_id/api/kindle_user_login";

static const QString MI_ACCOUNT_SERVICE_LOGIN_URI = "https://account.xiaomi.com/pass/serviceLogin";
static const QString MI_ACCOUNT_SERVICE_LOGIN_AUTH_URI = "https://account.xiaomi.com/pass/serviceLoginAuth";
static const QString MI_ACCOUNT_REGISTERED_CALLBACK_URI = "http://login.dushu.xiaomi.com/dk_id/api/checkin";

static qreal ZOOM_FACTOR = 1.5;

class DuokanServerConfiguration
{
public:
    static bool isOnline()
    {
        return is_testing_server_;
    }

    static QString xiaomiCheckinUrl()
    {
        if (is_testing_server_)
            return TEST_HOST + XIAOMI_CHECKIN;
        else
            return LOGIN_HOST + XIAOMI_CHECKIN;
    }

    static QString xiaomiExchangeUrl()
    {
        if (is_testing_server_)
            return TEST_HOST + XIAOMI_EXCHANGE;
        else
            return LOGIN_HOST + XIAOMI_EXCHANGE;
    }

    static QString xiaomiWebRegisterUrl()
    {
        if (is_testing_server_)
            return (TEST_HOST + XIAOMI_WEB_REGISTER);
        else
            return (LOGIN_HOST + XIAOMI_WEB_REGISTER);

    }

    static QString xiaomiFollowupUrl()
    {
        QString str_url;
        if (is_testing_server_)
            str_url = (TEST_HOST + XIAOMI_WEB_FOLLOWUP);
        else
            str_url = (LOGIN_HOST + XIAOMI_WEB_FOLLOWUP);

        //QString transfered_url = UrlEncode(str_url.toStdString().c_str());
        return str_url;
    }

    static QString xiaomiAccountCallback()
    {
        if (is_testing_server_)
            return TEST_HOST + XIAOMI_CHECKIN;
        else
            return LOGIN_HOST + XIAOMI_CHECKIN;
    }

    static bool checkOnlineOrTestingUrl(const QString& ref_url)
    {
        QString online_url  = (LOGIN_HOST + XIAOMI_WEB_REGISTER);
        QString testing_url = (TEST_HOST + XIAOMI_WEB_REGISTER);
        if (ref_url.contains(online_url))
        {
            is_testing_server_ = false;
            return true;
        }
        if (ref_url.contains(testing_url))
        {
            is_testing_server_ = true;
            return true;
        }
        return false;
    }

public:
    static bool is_testing_server_;
};

bool DuokanServerConfiguration::is_testing_server_ = false;

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

qreal XiaomiAccountManager::getZoomFactor()
{
    return ZOOM_FACTOR;
}

QStringList XiaomiAccountManager::getServiceTokenFromCookies(const QList<QNetworkCookie>& cookies)
{
    for (int i = cookies.count() - 1; i >= 0; --i)
    {
        qDebug("Cookie Name:%s, Value:%s", qPrintable(cookies.at(i).name()), qPrintable(cookies.at(i).value()));
        if (cookies.at(i).name().startsWith("serviceToken"))
        {
            QString service_token = cookies.at(i).value();
            QChar quo('\"');
            service_token = service_token.remove(quo);
            QStringList service_info = service_token.split("ABCDFGXYZ");
            return service_info;
        }
    }
    return QStringList();
}

QString XiaomiAccountManager::getUserIdFromCookies(const QList<QNetworkCookie>& cookies)
{
    for (int i = cookies.count() - 1; i >= 0; --i)
    {
        qDebug("Cookie Name:%s, Value:%s", qPrintable(cookies.at(i).name()), qPrintable(cookies.at(i).value()));
        if (cookies.at(i).name().startsWith("userId"))
        {
            return cookies.at(i).value();
        }
    }
    return QString();
}

void XiaomiAccountManager::onLoadStarted()
{
    QUrl current_url = view_->url();
    const QString & my_url = current_url.toString();
    if (my_url.startsWith(DuokanServerConfiguration::xiaomiFollowupUrl()))
    {
        view_->stop();
        exchangeDuokanToken(current_url);
    }
    //else
    //{
    //    if (my_url.startsWith(DuokanServerConfiguration::xiaomiWebRegisterUrl()))
    //    {
    //        emit startLogin();
    //    }
    //    else if (my_url.startsWith(MI_ACCOUNT_SERVICE_LOGIN_URI))
    //    {
    //        emit startLoginAuth();
    //    }
    //}
}

void XiaomiAccountManager::onLoadFinished(bool ok)
{
    QUrl current_url = view_->url();
    QString my_url = current_url.toEncoded();
    static bool first_display = false;
    if (my_url.startsWith(MI_ACCOUNT_SERVICE_LOGIN_URI) && !first_display)
    {
        first_display = true;
        //emit loginPageLoadFinished(ok);
        emit pageChanged(tr("Please Login"));
    }
}

void XiaomiAccountManager::onUrlChanged(const QUrl& url)
{
    QString my_url = url.toEncoded();
    if (my_url.startsWith(DuokanServerConfiguration::xiaomiWebRegisterUrl()))
    {
        //emit startLogin();
        emit pageChanged(tr("Loading Login Page..."));
    }
    else if (my_url.startsWith(MI_ACCOUNT_SERVICE_LOGIN_URI))
    {
        if (my_url.startsWith(MI_ACCOUNT_SERVICE_LOGIN_AUTH_URI))
        {
            emit pageChanged(tr("Start Login..."));
        }
        else
        {
            emit startLoginAuth();
        }
    }
    else if (my_url.startsWith(MI_ACCOUNT_REGISTERED_CALLBACK_URI))
    {
        //emit startCheckin();
        emit pageChanged(tr("Check In..."));
        if (DuokanServerConfiguration::isOnline())
        {
            qDebug("Replace URL for internal testing");
            CookieJar* cookieJar = dynamic_cast<CookieJar*>(getAccessManagerInstance()->cookieJar());
            //QList<QNetworkCookie> cookies = cookieJar->cookiesForUrl(current_url);
            //user_id_ = getUserIdFromCookies(cookies);

            cookieJar->clear();
            QString targetUrl = my_url.replace(MI_ACCOUNT_REGISTERED_CALLBACK_URI,
                DuokanServerConfiguration::xiaomiAccountCallback());
            load(targetUrl);
        }
    }
    else if (my_url.startsWith(DuokanServerConfiguration::xiaomiFollowupUrl()))
    {
        //emit startExchangeToken();
        emit pageChanged(tr("Retrieving User Data..."));
        view_->stop();
        exchangeDuokanToken(url);
    }
    else
    {
        emit pageChanged(url.toString());
    }
}

bool XiaomiAccountManager::isXiaomiAccountPath(const QString& path)
{
    return DuokanServerConfiguration::checkOnlineOrTestingUrl(path);
}

void XiaomiAccountManager::connectWebView(QWebView* view)
{
    view_ = view;

    // Setup connections.
    connect(view_, SIGNAL(loadStarted()), this, SLOT(onLoadStarted()));
    connect(view_, SIGNAL(loadFinished(bool)), this, SLOT(onLoadFinished(bool)));
    connect(view_, SIGNAL(urlChanged(const QUrl&)), this, SLOT(onUrlChanged(const QUrl&)));

    // Use customized cookie jar
    CookieJar* xiaomi_cookie_jar = dynamic_cast<CookieJar*>(getAccessManagerInstance()->cookieJar());
    xiaomi_cookie_jar->setStorageName(QLatin1String("/xiaomi_cookies.ini"));
    xiaomi_cookie_jar->setSettingName(QLatin1String("xiaomi_cookies"));
    xiaomi_cookie_jar->load();
    if (xiaomi_cookie_jar->keepPolicy() != CookieJar::KeepUntilExit)
    {
        xiaomi_cookie_jar->setKeepPolicy(CookieJar::KeepUntilExit);
    }
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

        // Use default cookie jar
        CookieJar* xiaomi_cookie_jar = dynamic_cast<CookieJar*>(getAccessManagerInstance()->cookieJar());
        xiaomi_cookie_jar->reset();
    }
}


// Reference
/*SecretKeySpec skeySpec = new SecretKeySpec(raw, "AES");
Cipher cipher = Cipher.getInstance("AES/ECB/nopadding");// "算法/模式/补码方式"
cipher.init(Cipher.ENCRYPT_MODE, skeySpec);
byte[] encrypted = cipher.doFinal(sSrc.getBytes());
return Base64.encodeToString(encrypted, Base64.NO_WRAP);*/
bool XiaomiAccountManager::getEncryptedPasswordFromToken(const QString& token, QByteArray& output)
{
    QString service_token = token;
    QStringList sources = service_token.split(',');
    QString source = sources[0];
    char* password = "s87PfD3FczE5z01XaB6YacbG9lQc20A3";

    // Do AES encrypy
    AES_KEY aes_key;
 
	char in[1024]={0};
	char cipher[1024]={0};
	char text[1024]={0};

    int len = source.size();
	int rem = 16 - len % 16;
	int bits = strlen(password) * 8;
	
	memcpy(in, source.toAscii().constData(), len);
	memset(in + len, ' ', rem);
	len += rem;

	AES_set_encrypt_key((const unsigned char*)password, bits, &aes_key);
	AES_ecb_encrypt((const unsigned char*)in, (unsigned char*)cipher, len, &aes_key, 1);   // enc = 1, encrypt

    QByteArray cipherByteArray = QByteArray::fromRawData(cipher, len);
    output = cipherByteArray.toBase64();
    return true;
}

bool XiaomiAccountManager::exchangeDuokanToken(const QUrl& url)
{
    QList<QNetworkCookie> cookies = getAccessManagerInstance()->cookieJar()->cookiesForUrl(url);
    QStringList service_info = getServiceTokenFromCookies(cookies);
    if (!service_info.isEmpty())
    {
        user_id_ = service_info[0];
        service_token_ = service_info[1];
        QByteArray password;
        getEncryptedPasswordFromToken(service_token_, password);
        QString exchange_url = DuokanServerConfiguration::xiaomiExchangeUrl();
        QUrl url(guessUrlFromString(exchange_url));

        QByteArray post_data;
        post_data.append("uid")
                 .append("=")
                 .append(UrlEncode(user_id_.toStdString().c_str()))
                 .append("&");

        post_data.append("auth")
            .append("=")
            .append(UrlEncode(password.data()))
            .append("&");

        post_data.append("deviceid")
            .append("=")
            .append(DeviceInfo::GetDeviceID().c_str())
            .append("&");

        post_data.append("appid")
            .append("=")
            .append(UrlEncode("KindleUser"))
            .append("&");

        post_data.append("format")
            .append("=")
            .append(UrlEncode("json"));

        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
        request.setHeader(QNetworkRequest::ContentLengthHeader, post_data.size());

        qDebug("\nPost Data:%s\n", post_data.constData());
        reply_ = network_service::getAccessManagerInstance()->post(request, post_data);
        connectNetworkReply(reply_);
        return true;
    }
    return false;
}

void XiaomiAccountManager::connectNetworkReply(QNetworkReply* reply)
{
    if (reply != 0)
    {
        connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
                this, SLOT(onNetworkReplyError(QNetworkReply::NetworkError)), Qt::QueuedConnection);
        connect(reply, SIGNAL(finished()),
                this, SLOT(onNetworkReplyFinished()), Qt::QueuedConnection);
    }
}

void XiaomiAccountManager::onNetworkReplyError(QNetworkReply::NetworkError error)
{
    qDebug("Network Reply Error:%d", error);
}

void XiaomiAccountManager::onNetworkReplyFinished()
{
    QByteArray result = reply_->readAll();
    qDebug("Network Reply Finished:%s", qPrintable(result));
    bool ok = parseAndSave(result);
    emit loginFinished(ok);
}

void XiaomiAccountManager::load(const QString& path)
{
    QUrl my_url = guessUrlFromString(path);
    if (view_ != 0 && my_url.isValid())
    {
        view_->load(my_url);
    }
}

void XiaomiAccountManager::login(const QString& ref_url, const QString& option)
{
    if (DuokanServerConfiguration::checkOnlineOrTestingUrl(ref_url))
    {
        bool login_or_register = true;
        if (option.compare("login", Qt::CaseInsensitive) == 0)
        {
            login_or_register = true;
        }
        else if (option.compare("register", Qt::CaseInsensitive) == 0)
        {
            login_or_register = false;
        }

        QString login_str = generateXiaomiAccountLoginUrl();
        QUrl url = guessUrlFromString(login_str);

        QUrl followup_url = guessUrlFromString(DuokanServerConfiguration::xiaomiFollowupUrl());
        QByteArray followup_raw = followup_url.toEncoded();
        //QByteArray followup_raw(UrlEncodeForStdString(DuokanServerConfiguration::xiaomiFollowupUrl().toStdString().c_str()).c_str());

        url.addEncodedQueryItem("followup", followup_raw);
        url.addEncodedQueryItem("source", "wap");
        url.addEncodedQueryItem("login", login_or_register ? "1" : "0"); // 0 = register; 1 = login

        view_->load(url);
    }
}

bool XiaomiAccountManager::parseAndSave(const QByteArray& data)
{
    QString json_data(data);
 
    QScriptEngine se;
    QScriptValue sv = se.evaluate ("JSON.parse").call(QScriptValue(), QScriptValueList() << json_data);

    QVariant status_variant = sv.property("status").toVariant();
    QVariant token = sv.property("token").toVariant();
    return saveResults(status_variant, token);
}

bool XiaomiAccountManager::saveResults(const QVariant& status, const QVariant& token)
{
    QString message;
    int result = -1;
    QString token_str;
    if (status.type() == QVariant::Map)
    {
        QMap<QString, QVariant> status_map = status.toMap();
        if (status_map.find("msg") != status_map.end())
        {
            message = status_map["msg"].toString();
        }
        if (status_map.find("code") != status_map.end())
        {
            result = status_map["code"].toInt();
        }
    }

    if (token.type() == QVariant::String)
    {
        token_str = token.toString();
    }
    
    // save token to db
    token_.mutableToken() = token_str;
    token_.mutableUserId() = user_id_;
    QString code_str = QString("%1").arg(result);
    token_.mutableCode() = code_str;
    token_.mutableMessage() = message;

    TokenRing token_ring;
    token_ring.setXiaomiToken(token_);
    
    // save to file
    return token_.saveToFile();
}

}
