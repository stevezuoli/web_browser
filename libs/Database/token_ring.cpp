#include <stdlib.h>
#include "NetworkService/ns_utils.h"
#include "token_ring.h"

using namespace network_service;

namespace web_database
{

static const QString TOKEN_RING_HOME = "/var/local/";

TokenRing::TokenRing()
{
    open();

    EvernoteTokenDB::createTableIfNotExist(*database_);
    XiaomiTokenDB::createTableIfNotExist(*database_);
}

TokenRing::~TokenRing()
{
    close();
}

QString TokenRing::home()
{
#ifdef BUILD_FOR_ARM
    return TOKEN_RING_HOME;
#else
    return QDir::home().path();
#endif
}

bool TokenRing::open()
{
    if (!database_)
    {
        if (QSqlDatabase::contains("token_ring"))
        {
            database_.reset(new QSqlDatabase(QSqlDatabase::database("token_ring")));
        }
        else
        {
            database_.reset(new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", "token_ring")));
        }

        QString path = TokenRing::home();
        QDir dir(path);
        database_->setDatabaseName(dir.filePath("token_ring.db"));
    }
    return database_->open();
}

bool TokenRing::close()
{
    if (database_)
    {
        database_.reset(0);
        return true;
    }
    return false;
}

bool TokenRing::evernoteToken(EvernoteToken& token)
{
    return EvernoteTokenDB::token(*database_, token);
}

bool TokenRing::setEvernoteToken(const EvernoteToken& token)
{
    return EvernoteTokenDB::setToken(*database_, token);
}

bool TokenRing::xiaomiToken(XiaomiToken& token)
{
    return XiaomiTokenDB::token(*database_, token);
}

bool TokenRing::setXiaomiToken(const XiaomiToken& token)
{
    return XiaomiTokenDB::setToken(*database_, token);
}

}

