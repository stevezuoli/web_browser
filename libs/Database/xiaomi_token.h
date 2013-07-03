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
    
    const QString& code() const { return code_; }
    QString& mutableCode() { return code_; }
    
    const QString& message() const { return message_; }
    QString& mutableMessage() { return message_; }

    bool saveToFile();

private:
    QString user_id_;
    QString token_;
    QString code_;
    QString message_;
};
    
class XiaomiMigration
{
public:
    XiaomiMigration() {}
    ~XiaomiMigration() {}
    
    const QString& duokanId() const { return duokan_id_; }
    QString& mutableDuokanId() { return duokan_id_; }
    
    const QString& token() const { return token_; }
    QString& mutableToken() { return token_; }
    
    const QString& miId() const { return mi_id_; }
    QString& mutableMiId() { return mi_id_; }
    
    const QString& miDeviceId() const { return mi_device_id_; }
    QString& mutableDeviceId() { return mi_device_id_; }
    
    const QString& message() const { return message_; }
    QString& mutableMessage() { return message_; }

    bool saveToFile();

private:
    QString duokan_id_;
    QString mi_id_;
    QString mi_device_id_;
    QString token_;
    QString message_;
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

