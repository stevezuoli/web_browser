#include <stdlib.h>
#include "NetworkService/ns_utils.h"
#include "web_data.h"

using namespace network_service;

namespace web_database
{

static const int DEFAULT_CAPACITY = 20;

WebData::WebData()
: capacity_(DEFAULT_CAPACITY)
{
    open("web_data");
    makeSureTableExist(*database_);
}

WebData::~WebData()
{
    close();
}

/// The database is in home directory.
bool WebData::open(const QString & name)
{
    if (!database_)
    {
        database_.reset(new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", "application_data")));
        QString path("%1.db");
        path = path.arg(name);
#ifdef BUILD_FOR_ARM
        QDir storage_dir(getCacheLocation());
        database_->setDatabaseName(storage_dir.filePath(path));
#else
        database_->setDatabaseName(QDir::home().filePath(path));
#endif
    }
    return database_->open();
}

bool WebData::close()
{
    if (database_)
    {
        database_->close();
        database_.reset(0);
        QSqlDatabase::removeDatabase("application_data");
        return true;
    }
    return false;
}

void WebData::makeSureTableExist(QSqlDatabase& database)
{
    QSqlQuery query(database);
    query.exec("create table if not exists conf_data ("
                      "key integer primary key, "
                      "value blob "
                      ")");
}

/// Load configuration from database.
bool WebData::load(QVariantList& conf)
{
    QSqlQuery query(*database_);
    query.prepare( "select value from conf_data");

    if (!query.exec())
    {
        return false;
    }

    while (query.next())
    {
        QVariant var;
        QByteArray data = query.value(0).toByteArray();
        QBuffer buffer(&data);
        buffer.open(QIODevice::ReadOnly);
        QDataStream stream(&buffer);
        stream >> var;
        conf.prepend(var);
    }
    return true;
}

bool WebData::clear(QSqlDatabase& database)
{
    QSqlQuery query(database);
    query.prepare( "delete from conf_data");
    if (!query.exec())
    {
        qDebug() << query.lastError().text();
        return false;
    }
    return true;
}

bool WebData::save(const QVariantList & conf)
{
    clear(*database_);

    QSqlQuery query(*database_);

    query.prepare( "insert into conf_data (value) values(?)");

    // We only store the first maxCount pages.
    int index = 0;
    foreach(QVariant v, conf)
    {
        QByteArray data;
        QBuffer buffer(&data);
        buffer.open(QIODevice::WriteOnly);
        QDataStream stream(&buffer);
        stream << v;
        query.addBindValue(data);
        query.exec();

        // Check
        if (++index >= capacity())
        {
            break;
        }
    }
    return true;
}

void WebData::clear()
{
    clear(*database_);
}

WebThumbnail::WebThumbnail()
    : QMap<QString, QVariant>()
{
}

WebThumbnail::WebThumbnail( const QMap<QString, QVariant>& right )
    : QMap<QString, QVariant> ( right )
{
}

WebThumbnail::~WebThumbnail()
{
}

QSize WebThumbnail::size()
{
    return QSize(256, 256);
}

void WebThumbnail::toByteArray(QByteArray & data)
{
    QBuffer buffer(&data);
    buffer.open(QIODevice::WriteOnly);
    QDataStream stream(&buffer);
    stream << *this;
}

void WebThumbnail::fromByteArray(QByteArray & data)
{
    QBuffer buffer(&data);
    buffer.open(QIODevice::ReadOnly);
    QDataStream stream(&buffer);
    stream >> *this;
}

const QString WebThumbnail::title() const
{
     return value("title").toString();
}

void WebThumbnail::setTitle(const QString & title)
{
    insert("title", title);
}

const QUrl WebThumbnail::url() const
{
    return value("url").toUrl();
}

void WebThumbnail::setUrl(const QUrl & url)
{
    insert("url", url);
}

QImage WebThumbnail::image() const
{
    QImage t;
    t.loadFromData(value("thumbnail").toByteArray());
    return t;
}

void WebThumbnail::setImage(const QImage & image)
{
    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "png");
    insert("thumbnail", ba);
}

void WebThumbnail::updateAccessDateTime()
{
    setAccessDateTime(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm"));
}

QString WebThumbnail::accessDateTime() const
{
    return value("access").toString();
}

void WebThumbnail::setAccessDateTime(const QString & time)
{
    insert("access", time);
}

bool GreaterByAccessTime::operator()(const QVariant& a, const QVariant& b) const
{
    return (a.toMap().value("access").toString() > b.toMap().value("access").toString());
}

QString WebThumbnail::accessDate() const
{
    QStringList list = accessDateTime().split(' ');
    if (list.length() == 2)
    {
        return list[0];
    }

    return "";
}

QString WebThumbnail::accessTime() const
{
    QStringList list = accessDateTime().split(' ');
    if (list.length() == 2)
    {
        return list[1];
    }

    return "";
}

bool WebThumbnail::dateTimeGreaterThan(const QVariant& lhs, const QVariant& rhs)
{
    return ((const WebThumbnail&)(lhs)).accessDateTime() > ((const WebThumbnail&)(rhs)).accessDateTime();
}
}

