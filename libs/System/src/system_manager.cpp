#include "system_manager.h"
#ifdef BUILD_FOR_ARM
#include <QtGui/qwsdisplay_qws.h>
#include <QWSPointerCalibrationData>
#include <qwindowsystem_qws.h>
#endif
#include <iostream>
#include <QRegExp>

SystemManager::SystemManager()
{
}

SystemManager::~SystemManager()
{
}

void SystemManager::snapshot(const QString &path)
{
    screen_manager_.snapshot(path);
}

bool SystemManager::setGrayScale(int colors)
{
    return screen_manager_.setGrayScale(colors);
}

int SystemManager::grayScale()
{
    return screen_manager_.grayScale();
}

//SystemManagerAdaptor::SystemManagerAdaptor(QApplication &app)
//: QDBusAbstractAdaptor(&app)
//{
//#ifndef _WINDOWS
//    QDBusConnection::systemBus().registerService("com.duokan.service.system_manager");
//    QDBusConnection::systemBus().registerObject("/com/duokan/object/system_manager", &app);
//#else
//    QDBusConnection::sessionBus().registerService("com.duokan.service.system_manager");
//    QDBusConnection::sessionBus().registerObject("/com/duokan/object/system_manager", &app);
//#endif
//
//    // Install keyboard filter.
//#ifdef BUILD_FOR_ARM
//    //QWSServer::addKeyboardFilter(&keyboard_filter_);
//    //QWSServer::setBackground(QBrush(QPixmap(":/images/background_vertical.png")));
//#endif
//
//    // Post startup.
//    QTimer::singleShot(3500, this, SLOT(broadcastInitialSignals(void)));
//
//}
//
//SystemManagerAdaptor::~SystemManagerAdaptor()
//{
//}
