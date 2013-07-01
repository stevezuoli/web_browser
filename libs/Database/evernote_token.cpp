#include <stdlib.h>
#include "NetworkService/ns_utils.h"
#include "Device/fat.h"
#include "evernote_token.h"

using namespace network_service;

namespace web_database
{

static const QString TOKEN_PATH = "/var/local/";
static const QString EVERNOTE_DIR = "evernote";
    
static QMap<QByteArray, QByteArray> parseUrlFromByteArray(const QByteArray& url_bytes)
{
    QList<QByteArray> raw_list = url_bytes.split('&');
    QMap<QByteArray, QByteArray> splitted;
    foreach(QByteArray raw_str, raw_list)
    {
        QList<QByteArray> sub_split_list = raw_str.split('=');
        if (sub_split_list.size() > 1)
        {
            splitted[sub_split_list[0]] = sub_split_list[1];
        }
    }
    return splitted;
}

EvernoteToken::EvernoteToken(const QString& host, const QByteArray& raw_data)
    : raw_data_(raw_data)
    , host_(host)
{
    parse();
}

void EvernoteToken::parse()
{
    if (!raw_data_.isEmpty())
    {
        QMap<QByteArray, QByteArray> splitted = parseUrlFromByteArray(raw_data_);
        QByteArray raw_token = splitted["oauth_token"];
        token_ = QUrl::fromEncoded(raw_token).toString();

        expire_date_ = splitted["edam_expires"];
        user_id_ = splitted["edam_userId"];
        
        QByteArray raw_note_store_url = splitted["edam_noteStoreUrl"];
        note_store_url_ = QUrl::fromEncoded(raw_note_store_url).toString();

        QByteArray raw_web_api_url_prefix = splitted["edam_webApiUrlPrefix"];
        web_api_url_prefix_ = QUrl::fromEncoded(raw_web_api_url_prefix).toString();

        secret_ = splitted["oauth_token_secret"];
    }
}

EvernoteTokenDB::EvernoteTokenDB()
{
}

EvernoteTokenDB::~EvernoteTokenDB()
{
}

bool EvernoteTokenDB::createTableIfNotExist(QSqlDatabase& db)
{
    QSqlQuery query(db);
    bool ok = query.exec("create table if not exists evernote_tokens ("
                         "user_id text," //primary key?
                         "host text,"
                         "token text,"
                         "note_store_url text,"
                         "web_api_url_prefix text,"
                         "secret text,"
                         "expire_date text,"
                         "raw_data blob"
                         ")");
    return ok;
}

bool EvernoteTokenDB::token(QSqlDatabase& db, EvernoteToken& token)
{
    QSqlQuery query(db);
    query.prepare("select user_id, host, token, note_store_url, web_api_url_prefix, secret, expire_date, raw_data from evernote_tokens where host = ?");
    query.addBindValue(token.host());

    if (query.exec() && query.next())
    {
        int index = 0;
        token.mutableUserId() = query.value(index++).toString();
        token.mutableHost() = query.value(index++).toString();
        token.mutableToken() = query.value(index++).toString();
        token.mutableNoteStoreUrl() = query.value(index++).toString();
        token.mutableWebApiUrlPrefix() = query.value(index++).toString();
        token.mutableSecret() = query.value(index++).toString();
        token.mutableExpireDate() = query.value(index++).toString();
        token.mutableRawData() = query.value(index++).toByteArray();
        return true;
    }
    return false;
}

bool EvernoteTokenDB::setToken(QSqlDatabase & db, const EvernoteToken& token)
{
    QSqlQuery delete_old(db);
    delete_old.prepare( "delete from evernote_tokens where host = ?");
    delete_old.addBindValue(token.host());

    if (!delete_old.exec())
    {
        qDebug("Insert new evernote token");
    }
    else
    {
        qDebug("Update existed evernote token");
    }

    QSqlQuery query(db);
    query.prepare ("INSERT OR REPLACE into evernote_tokens "
                   "(user_id, host, token, note_store_url, web_api_url_prefix, secret, expire_date, raw_data)"
                   " values "
                   "(?, ?, ?, ?, ?, ?, ?, ?)");

    query.addBindValue(token.userId());
    query.addBindValue(token.host());
    query.addBindValue(token.token());
    query.addBindValue(token.noteStoreUrl());
    query.addBindValue(token.webApiUrlPrefix());
    query.addBindValue(token.secret());
    query.addBindValue(token.expireDate());
    query.addBindValue(token.rawData());
    return query.exec();
}
    
QString EvernoteToken::home()
{
#ifdef BUILD_FOR_ARM
    return TOKEN_PATH;
#else
    return QDir::home().path();
#endif
}

QString EvernoteToken::dirName()
{
    return EVERNOTE_DIR;
}

bool EvernoteToken::save()
{
    QDomDocument doc("EvernoteToken");
    QDomElement root = doc.createElement("EvernoteToken");
    doc.appendChild(root);
    
    QDomElement user_id_element = doc.createElement("userid");
    root.appendChild(user_id_element);
    QDomText user_id = doc.createTextNode(user_id_);
    user_id_element.appendChild(user_id);
    
    QDomElement host_element = doc.createElement("host");
    root.appendChild(host_element);
    QDomText host = doc.createTextNode(host_);
    host_element.appendChild(host);
    
    QDomElement token_element = doc.createElement("token");
    root.appendChild(token_element);
    QDomText token = doc.createTextNode(token_);
    token_element.appendChild(token);
    
    QDomElement expire_date_element = doc.createElement("expire_date");
    root.appendChild(expire_date_element);
    QDomText expire_date = doc.createTextNode(expire_date_);
    expire_date_element.appendChild(expire_date);
    
    QDomElement secret_element = doc.createElement("secret");
    root.appendChild(secret_element);
    QDomText secret = doc.createTextNode(secret_);
    secret_element.appendChild(secret);
    
    QString xml = doc.toString();
    qDebug("EvernoteToken:%s", qPrintable(xml));
    
    QString path = EvernoteToken::home();
    QDir dir(path);
    if (!dir.exists(EvernoteToken::dirName()))
    {
        if (!dir.mkdir(EvernoteToken::dirName()))
        {
            return false;
        }
    }
    
    if (dir.cd(EvernoteToken::dirName()))
    {
        // Change folder attribute.
        changeToHidden(dir.absolutePath().toLocal8Bit().constData());
        
        path = dir.filePath("evernote_token.xml");
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

}

