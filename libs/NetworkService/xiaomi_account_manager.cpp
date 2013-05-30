#include "xiaomi_account_manager.h"
#include "access_manager.h"
#include "cookie_jar.h"

#include "FbLib/DeviceInfo.h"
#include "AES/aes.h"
#include "Device/fat.h"

namespace network_service
{

#define TEST_SERVER

static const QString TOKEN_PATH = "/mnt/us/DK_System/xKindle/";
static const QString WEB_BROWSER_DIR = "web_browser";

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
        return i - 10 + 'A';
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

    static QString getTokenHome()
    {
#ifdef BUILD_FOR_ARM
        return TOKEN_PATH;
#else
        return QDir::home().path();
#endif
    }
};

XiaomiAccountManager::XiaomiAccountManager()
    : view_(0)
    , stoped_(false)
{
}

XiaomiAccountManager::~XiaomiAccountManager()
{
}

QString XiaomiAccountManager::generateXiaomiAccountLoginUrl()
{
    return DuokanServerConfiguration::xiaomiWebRegisterUrl(); //+ "?followup=" + DuokanServerConfiguration::xiaomiFollowupUrl();
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
    const QString & myUrl = current_url.toString();
    if (myUrl.startsWith(DuokanServerConfiguration::xiaomiFollowupUrl()))
    {
        view_->stop();
        exchangeDuokanToken(current_url);
    }
    else
    {
        if (myUrl.startsWith(DuokanServerConfiguration::xiaomiWebRegisterUrl()))
        {
            emit startLogin();
        }
        else if (myUrl.startsWith(MI_ACCOUNT_SERVICE_LOGIN_URI))
        {
            stoped_ = false;
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
        emit loginPageLoadFinished(ok);
    }
    else if (myUrl.startsWith(MI_ACCOUNT_REGISTERED_CALLBACK_URI) &&
        !DuokanServerConfiguration::isOnline())
    {
        if (stoped_)
        {
            return;
        }
        CookieJar* cookieJar = dynamic_cast<CookieJar*>(getAccessManagerInstance()->cookieJar());
        //QList<QNetworkCookie> cookies = cookieJar->cookiesForUrl(current_url);
        //user_id_ = getUserIdFromCookies(cookies);

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
        stoped_ = true;
        view_->stop();
        exchangeDuokanToken(url);
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
    parseAndSave(result);
}

void XiaomiAccountManager::load(const QString& path)
{
    QUrl my_url = guessUrlFromString(path);
    if (view_ != 0 && my_url.isValid())
    {
        view_->load(my_url);
    }
}

void XiaomiAccountManager::login(bool login_or_register)
{
    QString login_str = generateXiaomiAccountLoginUrl();
    QUrl url = guessUrlFromString(login_str);
    url.addEncodedQueryItem("followup", DuokanServerConfiguration::xiaomiFollowupUrl().toUtf8());
    url.addEncodedQueryItem("login", login_or_register ? "1" : "0"); // 0 = register; 1 = login

    QByteArray str = url.toEncoded();
    view_->load(url);
}

bool XiaomiAccountManager::parseAndSave(const QByteArray& data)
{
    QString json_data(data);
 
    QScriptEngine se;
    QScriptValue sv = se.evaluate ("JSON.parse").call(QScriptValue(), QScriptValueList() << json_data);

    QVariant status_variant = sv.property("status").toVariant();
    QVariant token = sv.property("token").toVariant();
    int result = -1;
    if (status_variant.type() == QVariant::Map)
    {
        QMap<QString, QVariant> status_map = status_variant.toMap();
        if (status_map.find("msg") != status_map.end())
        {
            QString message = status_map["msg"].toString();
        }
        if (status_map.find("code") != status_map.end())
        {
            result = status_map["code"].toInt();
        }
    }

    if (result == 0)
    {
        if (token.type() == QVariant::String)
        {
            QString token_str = token.toString();
            saveToken(token_str);
        }
    }
    return result == 0;
}

bool XiaomiAccountManager::saveToken(const QString& token)
{
    QDomDocument doc("XiaomiToken");
    QDomElement root = doc.createElement("XiaomiToken");
    doc.appendChild(root);

    QDomElement token_element = doc.createElement("token");
    root.appendChild(token_element);

    QDomText value = doc.createTextNode(token);
    token_element.appendChild(value);

    QString xml = doc.toString();
    qDebug("XiaomiToken:%s", qPrintable(xml));

    QString path = DuokanServerConfiguration::getTokenHome();
    QDir dir(path);
    if (!dir.exists(WEB_BROWSER_DIR))
    {
        if (!dir.mkdir(WEB_BROWSER_DIR))
        {
            return false;
        }
    }

    if (dir.cd(WEB_BROWSER_DIR))
    {
        // Change folder attribute.
        changeToHidden(dir.absolutePath().toLocal8Bit().constData());

        path = dir.filePath("token.xml");
        QFile file(path);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) 
        {
            QTextStream TextStream(&file);
            TextStream << xml;
            file.close();
            return true;
        }
    }
    return false;
}

}
