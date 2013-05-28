#ifndef XIAOMI_ACCOUNT_MANAGER_H_
#define XIAOMI_ACCOUNT_MANAGER_H_

#include <QtWebKit/QtWebKit>

namespace network_service
{

class XiaomiAccountManager : public QObject
{
    Q_OBJECT
public:
    XiaomiAccountManager(QWebView* view);
    virtual ~XiaomiAccountManager();

Q_SIGNALS:
    void startLogin();
    void startLoginAuth();
    void loginFinished(bool);

private Q_SLOTS:
    void onLoadStarted();
    void onLoadFinished(bool ok);
    void onUrlChanged(const QUrl& url);

private:
    QString getServiceTokenFromCookies(const QList<QNetworkCookie>& cookies);
    bool exchangeDuokanToken(const QString& serviceToken);

private:
    QWebView* view_;
};

};
#endif
