#ifndef TOKEN_RING_H_
#define TOKEN_RING_H_

#include "Base/base.h"
#include <QtCore/QtCore>
#include <QtSql/QtSql>
#include "evernote_token.h"
#include "xiaomi_token.h"

namespace web_database
{

class TokenRing
{
public:
    TokenRing();
    ~TokenRing();

    bool open();
    bool close();

    bool evernoteToken(EvernoteToken& token);
    bool setEvernoteToken(const EvernoteToken& token);

    bool xiaomiToken(XiaomiToken& token);
    bool setXiaomiToken(const XiaomiToken& token);

    static QString home();

private:
    scoped_ptr<QSqlDatabase> database_;
};

};

#endif  // TOKEN_RING_H_
