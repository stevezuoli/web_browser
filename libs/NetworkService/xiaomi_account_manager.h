#ifndef XIAOMI_ACCOUNT_MANAGER_H_
#define XIAOMI_ACCOUNT_MANAGER_H_

#include <QtWebKit/QtWebKit>
#include "Database/xiaomi_token.h"

using namespace web_database;
namespace network_service
{

class XiaomiAccountManager : public QObject
{
    Q_OBJECT
public:
    XiaomiAccountManager();
    virtual ~XiaomiAccountManager();

    static bool isXiaomiAccountPath(const QString& path);
    void connectWebView(QWebView* view);
    void disconnectWebView();

    QString generateXiaomiAccountLoginUrl();

    // true = login; false = register
    void login(const QString& ref_url, const QString& option);
    qreal getZoomFactor();
    bool exchangeDuokanToken(const QUrl& url);
    XiaomiToken& xiaomiToken() { return token_; }

Q_SIGNALS:
    //void startLogin();
    void startLoginAuth();
    //void startCheckin();
    //void startExchangeToken();
    void loginPageLoadFinished(bool);
    void loginFinished(bool);
    void pageChanged(const QString& message);

private Q_SLOTS:
    void onLoadStarted();
    void onLoadFinished(bool ok);
    void onUrlChanged(const QUrl& url);

    void onNetworkReplyError(QNetworkReply::NetworkError error);
    void onNetworkReplyFinished();

    bool getEncryptedPasswordFromToken(const QString& token, QByteArray& output);

private:
    void load(const QString& path);

private:
    void connectNetworkReply(QNetworkReply* reply);
    QStringList getServiceTokenFromCookies(const QList<QNetworkCookie>& cookies);
    QString getUserIdFromCookies(const QList<QNetworkCookie>& cookies);
    
    bool parseAndSave(const QByteArray& data);
    bool saveResults(const QVariant& status, const QVariant& token);
private:
    QWebView* view_;

    QNetworkReply *reply_;
    QString service_token_;
    QString user_id_;
    XiaomiToken token_;
};

};
#endif
