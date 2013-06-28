#include <QtGui>
#include <QString>
#include "evernote_application.h"

using namespace evernote_kindle;

int main(int argc, char * argv[])
{
    EvernoteApplication app(argc, argv);
    QString param;
    if (argc >= 2)
    {
        param = QString::fromLocal8Bit(argv[1]);
    }
    else{
        param = EvernoteApplication::defaultFolder();
    }
    
    if (param.compare("getuser") == 0){
        if (app.createSession()){
            app.getUser();
        }
    }
    else {
        if (app.open(param) && app.createSession()){
            app.exportAll();
        }
    }
    return 0;
}
