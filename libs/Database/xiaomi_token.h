#ifndef XIAOMI_TOKEN_H_
#define XIAOMI_TOKEN_H_

#include "Base/base.h"
#include <QtCore/QtCore>
#include <QtSql/QtSql>

namespace web_database
{

class XiaomiToken
{
public:
    XiaomiToken() {}
    ~XiaomiToken() {}

    const QString& userId() const { return user_id_; }
    QString& mutableUserId() { return user_id_; }

    const QString& token() const { return token_; }
    QString& mutableToken() { return token_; }

    static QString home();
    static QString dirName();

private:
    QString user_id_;
    QString token_;
};

class XiaomiTokenDB
{
public:
    XiaomiTokenDB();
    ~XiaomiTokenDB();

private:
    friend class TokenRing;

    static bool createTableIfNotExist(QSqlDatabase& db);
    static bool token(QSqlDatabase& db, XiaomiToken& token);
    static bool setToken(QSqlDatabase & db, const XiaomiToken& data);
};

};

#endif  // XIAOMI_TOKEN_

