#include "xiaomi_migration_manager.h"
#include "access_manager.h"
#include "cookie_jar.h"

#include "Device/DeviceInfo.h"
#include "Device/fat.h"
#include "Database/xiaomi_token.h"
#include "Database/token_ring.h"

#define TEST_SERVER

using namespace web_database;

namespace network_service
{

static const QString XIAOMI_HOST = "http://login.dushu.xiaomi.com";
static const QString SANDBOX_HOST = "http://dkmars";

static const QString MIGRATION_START_URI = "/dk_id/v0/migrate/kindle/start";
static const QString MIGRATION_OK_URI = "/dk_id/v0/migrate/kindle/ok";
static const QString MIGRATION_FAIL_URI = "/dk_id/v0/migrate/kindle/fail";
static const QString MIGRATION_CANCEL_URI = "/dk_id/v0/migrate/kindle/cancel";

static const qreal MIGRATION_ZOOM = 1.0;
    

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
    , config_(host)
{
}

XiaomiMigrationManager::~XiaomiMigrationManager()
{
}

bool XiaomiMigrationManager::isXiaomiMigrationPath(const QString& path)
{
    return path.endsWith(MIGRATION_START_URI);
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

bool XiaomiMigrationManager::start()
{
    QString start_uri = config_.migrationServerStart();
    QUrl url = guessUrlFromString(start_uri);

    // prepare POST content
    QByteArray post_data;
    /* TODO.
    post_data.append("uid")
    .append("=")
    .append(UrlEncode(user_id_.toStdString().c_str()))
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
    */ 
    
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setHeader(QNetworkRequest::ContentLengthHeader, post_data.size());
    
    qDebug("\nPost Data:%s\n", post_data.constData());
    view_->load(request, QNetworkAccessManager::PostOperation, post_data);
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


void XiaomiMigrationManager::onUrlChanged(const QUrl& url)
{
    QString my_url = url.toEncoded();
    if (my_url.startsWith(config_.migrationServerOK()))
    {
        emit pageChanged(tr("Migration Succeeded"));
        emit migrationSucceeded();
    }
    else if (my_url.startsWith(config_.migrationServerFail()))
    {
        emit pageChanged(tr("Migration Failed"));
        emit migrationFailed();
    }
    else if (my_url.startsWith(config_.migrationServerCancel()))
    {
        emit pageChanged(tr("Migration Canceled"));
        emit migrationCanceled();
    }
    else
    {
        emit pageChanged(url.toString());
    }
}

void XiaomiMigrationManager::onMigrationSucceeded()
{
}

void XiaomiMigrationManager::onMigrationCanceled()
{
    
}

void XiaomiMigrationManager::onMigrationFailed()
{
    
}

}