#include "xiaomi_migration_manager.h"
#include "xiaomi_account_manager.h"
#include "access_manager.h"
#include "cookie_jar.h"
#include "ns_utils.h"

#include "Device/DeviceInfo.h"
#include "Device/fat.h"

#define TEST_SERVER

using namespace web_database;

namespace network_service
{

static const QString DUOKAN_HOST = "http://book.duokan.com";
static const QString SANDBOX_HOST = "http://dkmars";

static const QString MIGRATION_START_URI = "/dk_id/migrate/kindle/start";
static const QString MIGRATION_OK_URI = "/dk_id/migrate/kindle/success";
static const QString MIGRATION_FAIL_URI = "/dk_id/migrate/kindle/fail";
static const QString MIGRATION_CANCEL_URI = "/dk_id/migrate/kindle/cancel";
static const QString MI_DUSHU_URI = "http://login.dushu.xiaomi.com";

static const QString MIGRATION_COOKIE_CHECKPOINT = "/dk_id/migrate";
static const qreal MIGRATION_ZOOM = 1.0;

static const QString MIGRATION_MSG_SUCCEED = "succeed";
static const QString MIGRATION_MSG_CANCELED = "canceled";
static const QString MIGRATION_MSG_FAILED = "failed";

QString MigrationServerConfiguration::migrationServerStart()
{
    return host_ + MIGRATION_START_URI;
}

QString MigrationServerConfiguration::migrationServerOK()
{
    return host_ + MIGRATION_OK_URI;
}

QString MigrationServerConfiguration::migrationServerFail()
{
    return host_ + MIGRATION_FAIL_URI;
}

QString MigrationServerConfiguration::migrationServerCancel()
{
    return host_ + MIGRATION_CANCEL_URI;
}

XiaomiMigrationManager::XiaomiMigrationManager(const QString& host)
    : view_(0)
    , account_manager_(new XiaomiAccountManager)
    , config_(host)
{
    connect(this, SIGNAL(migrationSucceeded()), SLOT(onMigrationSucceeded()));
    connect(this, SIGNAL(migrationCanceled()), SLOT(onMigrationCanceled()));
    connect(this, SIGNAL(migrationFailed()), SLOT(onMigrationFailed()));

    connect(account_manager_.get(), SIGNAL(pageChanged(const QString&)), SIGNAL(pageChanged(const QString&)));
    connect(account_manager_.get(), SIGNAL(loginFinished(bool)), this, SLOT(onXiaomiExchangeFinished(bool)));
}

XiaomiMigrationManager::~XiaomiMigrationManager()
{
}

bool XiaomiMigrationManager::isXiaomiMigrationPath(const QString& path)
{
    return path.endsWith(MIGRATION_START_URI);
}
    
QString XiaomiMigrationManager::getHostFromPath(const QString& path)
{
    int idx = path.indexOf(MIGRATION_START_URI);
    if (idx > 0)
    {
        return path.left(idx);
    }
    return QString();
}

void XiaomiMigrationManager::connectWebView(QWebView* view)
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

void XiaomiMigrationManager::disconnectWebView()
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

qreal XiaomiMigrationManager::getZoomFactor()
{
    return MIGRATION_ZOOM;
}

bool XiaomiMigrationManager::start(const QString& duokan_account_id)
{
    if (duokan_account_id.isEmpty())
    {
        return false;
    }
    duokan_account_id_ = duokan_account_id;
    XiaomiMigration::removeFile();

    QString start_uri = config_.migrationServerStart();
    QUrl url = guessUrlFromString(start_uri);
    addCookiesForEntry(url);
    view_->load(url);
    return true;
}
    
void XiaomiMigrationManager::addCookiesForEntry(const QUrl& url)
{
    CookieJar* xiaomi_cookie_jar = dynamic_cast<CookieJar*>(getAccessManagerInstance()->cookieJar());
    QList<QNetworkCookie> cookies = xiaomi_cookie_jar->cookiesForUrl(url);
    
    // add new cookies for app id and device id
    QByteArray app_id_name("app_id");
    QByteArray kindle_user("KindleUser");
    QNetworkCookie app_id_cookie(app_id_name, kindle_user);
    app_id_cookie.setDomain(".duokan.com");
    app_id_cookie.setPath("/");
    
    QByteArray device_id_name("device_id");
    QByteArray device_id(DeviceInfo::GetDeviceID().c_str());
    QNetworkCookie device_id_cookie(device_id_name, device_id);
    device_id_cookie.setDomain(".duokan.com");
    device_id_cookie.setPath("/");

    QByteArray user_id_name("user_id");
    QNetworkCookie user_id_cookie(user_id_name, duokan_account_id_.toLocal8Bit());
    user_id_cookie.setDomain(".duokan.com");
    user_id_cookie.setPath("/");
    
    cookies.push_back(app_id_cookie);
    cookies.push_back(device_id_cookie);
    cookies.push_back(user_id_cookie);
    xiaomi_cookie_jar->setCookiesFromUrl(cookies, url);
}
    
bool XiaomiMigrationManager::loadDataFromCookies(const QUrl& url)
{
    CookieJar* xiaomi_cookie_jar = dynamic_cast<CookieJar*>(getAccessManagerInstance()->cookieJar());
    QList<QNetworkCookie> cookies = xiaomi_cookie_jar->cookiesForUrl(url);
    
    foreach(QNetworkCookie cookie, cookies)
    {
        if (cookie.name() == "token")
        {
            migration_result_.mutableToken() = QString::fromLocal8Bit(cookie.value().data());
        }
        else if (cookie.name() == "device_id")
        {
            migration_result_.mutableDeviceId() = QString::fromLocal8Bit(cookie.value().data());
        }
        else if (cookie.name() == "user_id")
        {
            QString decoded_user_id = UrlDecode(cookie.value().data());
            migration_result_.mutableMiId() = decoded_user_id;
        }
        else if (cookie.name() == "migrated_user_id")
        {
            QString decoded_migrated_user_id = UrlDecode(cookie.value().data());
            migration_result_.mutableDuokanId() = decoded_migrated_user_id;
        }
    }
    return true;
}

void XiaomiMigrationManager::onLoadStarted()
{
    // TODO.
}

void XiaomiMigrationManager::onLoadFinished(bool ok)
{
    // TODO.
}

void XiaomiMigrationManager::onXiaomiExchangeFinished(bool ok)
{
    migration_result_.mutableToken() = account_manager_->xiaomiToken().token();
    migration_result_.mutableMiId()  = account_manager_->xiaomiToken().userId();
    if (ok)
        emit migrationSucceeded();
    else
        emit migrationFailed();
}

void XiaomiMigrationManager::exchangeXiaomiToken()
{
    QUrl dushu_url = guessUrlFromString(MI_DUSHU_URI);
    account_manager_->exchangeDuokanToken(dushu_url);
}

void XiaomiMigrationManager::onUrlChanged(const QUrl& url)
{
    QString my_url = url.toEncoded();
    qDebug("\nUrl Changed:%s\n", qPrintable(my_url));
    if (my_url.contains(config_.migrationServerOK()))
    {
        view_->stop();
        loadDataFromCookies(url);
        exchangeXiaomiToken();
        migration_result_.mutableMessage() = MIGRATION_MSG_SUCCEED;
        emit pageChanged(tr("Migration Succeeded"));
    }
    else if (my_url.contains(config_.migrationServerFail()))
    {
        loadDataFromCookies(url);
        migration_result_.mutableMessage() = MIGRATION_MSG_FAILED;
        emit pageChanged(tr("Migration Failed"));
        emit migrationFailed();
    }
    else if (my_url.contains(config_.migrationServerCancel()))
    {
        loadDataFromCookies(url);
        migration_result_.mutableMessage() = MIGRATION_MSG_CANCELED;
        emit pageChanged(tr("Migration Canceled"));
        emit migrationCanceled();
    }
    else if (my_url.endsWith(MIGRATION_COOKIE_CHECKPOINT))
    {
        loadDataFromCookies(url);
    }
    else
    {
        emit pageChanged(url.toString());
    }
}

void XiaomiMigrationManager::onMigrationSucceeded()
{
    saveResult();
}

void XiaomiMigrationManager::onMigrationCanceled()
{
    saveResult();
}

void XiaomiMigrationManager::onMigrationFailed()
{
    saveResult();
}

void XiaomiMigrationManager::saveResult()
{
    migration_result_.saveToFile();
    //if (migration_result_.message() == MIGRATION_MSG_SUCCEED)
    //{
    //    XiaomiToken mi_token;
    //    mi_token.mutableToken() = migration_result_.token();
    //    mi_token.mutableMessage() = migration_result_.message();
    //    mi_token.mutableUserId() = migration_result_.miId();
    //    mi_token.mutableCode() = QString("%1").arg(0);
    //    mi_token.saveToFile();
    //}
}

}