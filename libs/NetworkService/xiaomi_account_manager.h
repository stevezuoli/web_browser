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
    void loginFinished(bool);

private Q_SLOTS:
    void onLoadStarted();
    void onLoadFinished(bool ok);
    void onUrlChanged(const QUrl& url);

private:
    void load(const QString& path);

private:
    QString getServiceTokenFromCookies(const QList<QNetworkCookie>& cookies);
    bool exchangeDuokanToken(const QString& serviceToken);

private:
    QWebView* view_;
};

};
#endif
