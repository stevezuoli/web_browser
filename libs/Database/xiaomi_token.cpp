#include <stdlib.h>
#include "NetworkService/ns_utils.h"
#include "xiaomi_token.h"

using namespace network_service;

namespace web_database
{

static const QString TOKEN_PATH = "/var/local/";
static const QString XIAOMI_DIR = "xiaomi";

QString XiaomiToken::home()
{
#ifdef BUILD_FOR_ARM
        return TOKEN_PATH;
#else
        return QDir::home().path();
#endif
}

QString XiaomiToken::dirName()
{
    return XIAOMI_DIR;
}

XiaomiTokenDB::XiaomiTokenDB()
{
}

XiaomiTokenDB::~XiaomiTokenDB()
{
}

bool XiaomiTokenDB::createTableIfNotExist(QSqlDatabase& db)
{
    QSqlQuery query(db);
    bool ok = query.exec("create table if not exists xiaomi_tokens ("
                         "user_id text primary key,"
                         "token text"
                         ")");
    return ok;
}

bool XiaomiTokenDB::token(QSqlDatabase& db, XiaomiToken& token)
{
    QSqlQuery query(db);
    query.prepare("select user_id, token from xiaomi_tokens where user_id = ? ");
    query.addBindValue(token.userId());

    if (query.exec() && query.next())
    {
        int index = 0;
        token.mutableUserId() = query.value(index++).toString();
        token.mutableToken() = query.value(index++).toString();
        return true;
    }
    return false;
}

bool XiaomiTokenDB::setToken(QSqlDatabase & db, const XiaomiToken& token)
{
    QSqlQuery delete_old(db);
    delete_old.prepare( "delete from xiaomi_tokens where user_id = ? ");
    delete_old.addBindValue(token.userId());

    if (!delete_old.exec())
    {
        qDebug("Insert new xiaomi token");
    }
    else
    {
        qDebug("Update existed xiaomi token");
    }

    QSqlQuery query(db);
    query.prepare ("INSERT OR REPLACE into xiaomi_tokens "
                   "(user_id, token)"
                   " values "
                   "(?, ?)");
    query.addBindValue(token.userId());
    query.addBindValue(token.token());
    return query.exec();
}

}

