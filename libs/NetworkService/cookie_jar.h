#ifndef COOKIE_JAR_H_
#define COOKIE_JAR_H_

#include <QtNetwork/QNetworkCookieJar>
#include "auto_saver.h"

namespace network_service
{

class CookieJar : public QNetworkCookieJar
{
    Q_OBJECT
    Q_PROPERTY(AcceptPolicy acceptPolicy READ acceptPolicy WRITE setAcceptPolicy)
    Q_PROPERTY(KeepPolicy keepPolicy READ keepPolicy WRITE setKeepPolicy)
    Q_PROPERTY(QStringList blockedCookies READ blockedCookies WRITE setBlockedCookies)
    Q_PROPERTY(QStringList allowedCookies READ allowedCookies WRITE setAllowedCookies)
    Q_PROPERTY(QStringList allowForSessionCookies READ allowForSessionCookies WRITE setAllowForSessionCookies)
    Q_ENUMS(KeepPolicy)
    Q_ENUMS(AcceptPolicy)

public:
    enum AcceptPolicy {
        AcceptAlways,
        AcceptNever,
        AcceptOnlyFromSitesNavigatedTo
    };

    enum KeepPolicy {
        KeepUntilExpire,
        KeepUntilExit,
        KeepUntilTimeLimit
    };

public:
    CookieJar(QObject *parent = 0,
        const QLatin1String &storage_name = QLatin1String("/cookies.ini"),
        const QLatin1String &setting_name = QLatin1String("cookies"));
    ~CookieJar();

    QList<QNetworkCookie> cookiesForUrl(const QUrl &url) const;
    bool setCookiesFromUrl(const QList<QNetworkCookie> &cookieList, const QUrl &url);

    AcceptPolicy acceptPolicy() const;
    void setAcceptPolicy(AcceptPolicy policy);

    KeepPolicy keepPolicy() const;
    void setKeepPolicy(KeepPolicy policy);

    QStringList blockedCookies() const;
    QStringList allowedCookies() const;
    QStringList allowForSessionCookies() const;

    void setBlockedCookies(const QStringList &list);
    void setAllowedCookies(const QStringList &list);
    void setAllowForSessionCookies(const QStringList &list);

    void setStorageName(const QString &storage_name) { storage_name_ = storage_name; }
    void setSettingName(const QString &setting_name) { setting_name_ = setting_name; }
    void load();
    void reset();

Q_SIGNALS:
    void cookiesChanged();

public Q_SLOTS:
    void clear();
    void loadSettings();

private Q_SLOTS:
    void save();

private:
    void purgeOldCookies();

private:
    bool         loaded_;
    AutoSaver    *save_timer_;
    AcceptPolicy accept_cookies_;
    KeepPolicy   keep_cookies_;

    QString storage_name_;
    QString setting_name_;
    QStringList  exceptions_block_;
    QStringList  exceptions_allow_;
    QStringList  exceptions_allow_for_session_;
};

};

#endif
