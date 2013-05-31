#include <QtGui>
#include <QString>
#include "web_application.h"
#include "System/inc/system_manager.h"
#include "Device/device.h"

using namespace webbrowser;

int main(int argc, char * argv[])
{
    Device::instance();
    SystemManager system;
    WebApplication app(argc, argv);
    //WebApplicationAdaptor adaptor(&app);
    
    QString path;
    if (argc >= 2)
    {
        path = QString::fromLocal8Bit(argv[1]);
    }

    Q_INIT_RESOURCE(res);
    //adaptor.open(path);
    app.open();
    int ret = app.exec();
    return ret;
}
