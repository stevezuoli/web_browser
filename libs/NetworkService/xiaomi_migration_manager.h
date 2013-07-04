#ifndef XIAOMI_MIGRATION_MANAGER_H_
#define XIAOMI_MIGRATION_MANAGER_H_

#include <QtWebKit/QtWebKit>
#include "Database/xiaomi_token.h"
#include "Database/token_ring.h"

using namespace web_database;

namespace network_service
{

class MigrationServerConfiguration
{
public:
    MigrationServerConfiguration(const QString& host) : host_(host) {}
    ~MigrationServerConfiguration() {}
    
    QString migrationServerStart();
    QString migrationServerOK();
    QString migrationServerFail();
    QString migrationServerCancel();

private:
    QString host_;
};

class XiaomiMigrationManager : public QObject
{
    Q_OBJECT
public:
    XiaomiMigrationManager(const QString& host);
    virtual ~XiaomiMigrationManager();

    static bool isXiaomiMigrationPath(const QString& path);
    static QString getHostFromPath(const QString& path);

    void connectWebView(QWebView* view);
    void disconnectWebView();
    bool start();
    qreal getZoomFactor();

Q_SIGNALS:
    void pageChanged(const QString& message);
    void migrationSucceeded();
    void migrationCanceled();
    void migrationFailed();

private Q_SLOTS:
    void onLoadStarted();
    void onLoadFinished(bool ok);
    void onUrlChanged(const QUrl& url);

    void onMigrationSucceeded();
    void onMigrationCanceled();
    void onMigrationFailed();

private:
    void addCookiesForEntry(const QUrl& url);
    void saveResult();
    bool loadDataFromCookies(const QUrl& url);
    
private:
    QWebView* view_;
    MigrationServerConfiguration config_;
    XiaomiMigration migration_result_;
};

};
#endif
