#ifndef XIAOMI_ACCOUNT_MANAGER_H_
#define XIAOMI_ACCOUNT_MANAGER_H_

#include <QtWebKit/QtWebKit>

namespace network_service
{

class XiaomiAccountManager : public QObject
{
    Q_OBJECT
public:
    XiaomiAccountManager();
    virtual ~XiaomiAccountManager();

    bool isXiaomiAccountPath(const QString& path);
    void connectWebView(QWebView* view);
    void disconnectWebView();

    QString generateXiaomiAccountLoginUrl();
    void login();

Q_SIGNALS:
    void startLogin();
    void startLoginAuth();
    void loginPageLoadFinished(bool);

private Q_SLOTS:
    void onLoadStarted();
    void onLoadFinished(bool ok);
    void onUrlChanged(const QUrl& url);

    void onNetworkReplyError(QNetworkReply::NetworkError error);
    void onNetworkReplyFinished();

    QByteArray getEncryptedPasswordFromToken(const QString& token);

private:
    void load(const QString& path);

private:
    void connectNetworkReply(QNetworkReply* reply);
    QStringList getServiceTokenFromCookies(const QList<QNetworkCookie>& cookies);
    QString getUserIdFromCookies(const QList<QNetworkCookie>& cookies);
    bool exchangeDuokanToken(const QUrl& url);

private:
    QWebView* view_;

    QNetworkReply *reply_;
    QString service_token_;
    QString user_id_;

    bool stoped_;
};

};
#endif
