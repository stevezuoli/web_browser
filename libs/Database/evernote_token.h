#ifndef EVERNOTE_TOKEN_H_
#define EVERNOTE_TOKEN_H_

#include "Base/base.h"
#include <QtCore/QtCore>
#include <QtSql/QtSql>

namespace web_database
{

class EvernoteToken
{
public:
    EvernoteToken() {}
    EvernoteToken(const QString& host, const QByteArray& raw_data);
    ~EvernoteToken() {}

    const QByteArray& rawData() const { return raw_data_; }
    QByteArray& mutableRawData() { return raw_data_; }

    const QString& noteStoreUrl() const { return note_store_url_; }
    QString& mutableNoteStoreUrl() { return note_store_url_; }

    const QString& webApiUrlPrefix() const { return web_api_url_prefix_; }
    QString& mutableWebApiUrlPrefix() { return web_api_url_prefix_; }

    const QString& userId() const { return user_id_; }
    QString& mutableUserId() { return user_id_; }

    const QString& host() const { return host_; }
    QString& mutableHost() { return host_; }

    const QString& token() const { return token_; }
    QString& mutableToken() { return token_; }

    const QString& expireDate() const { return expire_date_; }
    QString& mutableExpireDate() { return expire_date_; }

    const QString& secret() const { return secret_; }
    QString& mutableSecret() { return secret_; }

    bool isValid() { return !token_.isEmpty(); }

    bool save();
    static QString home();
    static QString dirName();
    
private:
    void parse();

private:
    QByteArray raw_data_;
    QString note_store_url_;
    QString web_api_url_prefix_;

    QString user_id_;
    QString host_;
    QString token_;
    QString expire_date_;
    QString secret_;
};

class EvernoteTokenDB
{
public:
    EvernoteTokenDB();
    ~EvernoteTokenDB();

private:
    friend class TokenRing;

    static bool createTableIfNotExist(QSqlDatabase& db);
    static bool token(QSqlDatabase& db, EvernoteToken& token);
    static bool setToken(QSqlDatabase & db, const EvernoteToken& token);
};

};

#endif  // EVERNOTE_TOKEN_H_
