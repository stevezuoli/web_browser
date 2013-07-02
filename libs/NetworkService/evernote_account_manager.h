#ifndef EVERNOTE_ACCOUNT_MANAGER_H_
#define EVERNOTE_ACCOUNT_MANAGER_H_

#include "Base/base.h"
#include <QtWebKit/QtWebKit>
#include <oauth/src/kqoauthmanager.h>
#include <oauth/src/kqoauthrequest.h>

namespace network_service
{

class EvernoteAccountManager : public QObject
{
    Q_OBJECT
public:
    EvernoteAccountManager();
    virtual ~EvernoteAccountManager();

    static bool isEvernotePath(const QString& path);
    void login(const QString& option);
    void connectWebView(QWebView* view);
    void disconnectWebView();

Q_SIGNALS:
    void askForAuthorization(QString auth_url);
    void loginFinished(bool);
    void pageChanged(const QString& message);

private Q_SLOTS:
    void onRequestReady(QByteArray reply);
    void onAccessTokenReady(QByteArray reply);
    void onAuthorization(QString auth_url);
    void onUrlChanged(const QUrl& url);

private:
    void askForRequestToken(const QString& option);
    void askForAccessToken();
    bool parseRequestTokenAndSecret(const QByteArray& reply);
    bool parseVerifier(const QString& url_str);

private:
    QWebView* view_;
    scoped_ptr<KQOAuthManager> oauth_manager_;
    scoped_ptr<KQOAuthRequest> oauth_request_;

    QString user_option_;

    QString consumer_key_;
    QString consumer_secret_;
    QString request_token_;
    QString request_token_secret_;
    
    QString request_uri_;
    QString authorization_uri_;
    QString callback_uri_;
    QString verifier_;
};

};
#endif
