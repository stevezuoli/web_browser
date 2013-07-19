#include <QtGui>
#include <QString>
#include <QScreenCursor>
#include "web_application.h"
#include "System/inc/system_manager.h"
#include "Device/device.h"

using namespace webbrowser;

int main(int argc, char * argv[])
{
#ifdef BUILD_FOR_ARM
    if (QScreenCursor::instance())
    {
        QScreenCursor::instance()->hide();
    }
    QWSServer::setCursorVisible(false);
#endif

    Device::instance();
#ifndef Q_WS_QWS
#ifndef WIN32
    int ch;
    while ((ch = getopt(argc, argv, "-ud:")) != -1)
    {
       switch (ch)
       {
           case 'u':
           case '?':
               qDebug("Usage:\n -d: SetDeviceType, eg. -d k3/k4/kt/kp");
               return 0;
           case 'd':
               qDebug("Device set to %s", optarg);
               if (strcmp(optarg, "k3") == 0)
               {
                   Device::setModel(Device::K3);
               }
               else if (strcmp(optarg, "k4") == 0)
               {
                   Device::setModel(Device::K4NT);
               }
               else if (strcmp(optarg, "k4b") == 0)
               {
                   Device::setModel(Device::K4NTB);
               }
               else if (strcmp(optarg, "kt") == 0)
               {
                   Device::setModel(Device::KT);
               }
               else if (strcmp(optarg, "kp") == 0)
               {
                   Device::setModel(Device::KPW);
               }
       }
    }
#endif
#endif
    Q_INIT_RESOURCE(res);
    
#ifdef BUILD_FOR_ARM
    if (QScreenCursor::instance())
    {
        QScreenCursor::instance()->set(QImage(":/res/cursor.png"), 0, 0);
        QScreenCursor::instance()->show();
        QWSServer::setCursorVisible(true);
    }
#endif

    WebApplication app(argc, argv);
    //WebApplicationAdaptor adaptor(&app);
    
    // initialize system manager. TO remove to standalone server process in the future
    SystemManager::instance();

    QString path;
    if (argc >= 2)
    {
        path = QString::fromLocal8Bit(argv[1]);
    }

    //adaptor.open(path);
    app.open();
    int ret = app.exec();
    return ret;
}
