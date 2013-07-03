#include <stdlib.h>
#include "NetworkService/ns_utils.h"
#include "xiaomi_token.h"
#include "xml_utils.h"
#include "Device/fat.h"

using namespace network_service;

namespace web_database
{

static const QString TOKEN_PATH = "/var/local/";
static const QString XIAOMI_DIR = "xiaomi";

static QString miTokenHome()
{
#ifdef BUILD_FOR_ARM
        return TOKEN_PATH;
#else
        return QDir::home().path();
#endif
}

static QString miTokenDir()
{
    return XIAOMI_DIR;
}
    
bool XiaomiToken::saveToFile()
{
    QDomDocument doc("XiaomiToken");
    QDomElement root = doc.createElement("XiaomiToken");
    doc.appendChild(root);

    appendXmlTextNode("id", user_id_, doc, root);
    appendXmlTextNode("code", code_, doc, root);
    appendXmlTextNode("msg", message_, doc, root);
    appendXmlTextNode("token", token_, doc, root);

    QString xml = doc.toString();
    qDebug("XiaomiToken:%s", qPrintable(xml));
    
    QString path = miTokenHome();
    QDir dir(path);
    if (!dir.exists(miTokenDir()))
    {
        if (!dir.mkdir(miTokenDir()))
        {
            return false;
        }
    }
    
    if (dir.cd(miTokenDir()))
    {
        // Change folder attribute.
        changeToHidden(dir.absolutePath().toLocal8Bit().constData());
        path = dir.filePath("token.xml");
        QFile file(path);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream TextStream(&file);
            TextStream << xml;
            file.close();
            return true;
        }
    }
    return false;
}

bool XiaomiMigration::saveToFile()
{
    
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

