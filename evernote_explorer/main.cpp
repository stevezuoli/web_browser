#include <QtGui>
#include <QString>
#include "evernote_application.h"

using namespace evernote_kindle;

int main(int argc, char * argv[])
{
    EvernoteApplication app(argc, argv);
    app.exportAll();
    return 0;
}
