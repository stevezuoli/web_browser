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
    //set font for QWebView
    app.setFont(QFont("Arial", 20, QFont::Bold));
    //adaptor.open(path);
    app.open(path);
    int ret = app.exec();
    return ret;
}
