#include <QtGui>
#include "web_application.h"
#include "view.h"
#include "System/inc/system_manager.h"

using namespace webbrowser;

int main(int argc, char * argv[])
{
    SystemManager system;
    WebApplication app(argc, argv);
    //WebApplicationAdaptor adaptor(&app);

    QString path;
    if (argc >= 2)
    {
        path = QString::fromLocal8Bit(argv[1]);
    }

    //adaptor.open(path);
    app.open(path);
    int ret = app.exec();
    return ret;
}
