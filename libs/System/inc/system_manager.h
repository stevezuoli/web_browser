#ifndef SYSTEM_MANAGER_ADAPTOR_H_
#define SYSTEM_MANAGER_ADAPTOR_H_

#include <QtGui/QtGui>
#include <QtDBus/QtDBus>
#include "screen_manager.h"


class SystemManager : public QObject
{
    Q_OBJECT
public:
    SystemManager();
    virtual ~SystemManager();

public Q_SLOTS:
    void snapshot(const QString &path);
    bool setGrayScale(int colors);
    int grayScale();

public:
    ScreenManager screen_manager_;
};

/// Device manager which reports device states.
//class SystemManagerAdaptor : public QDBusAbstractAdaptor
//{
//    Q_OBJECT
//    Q_CLASSINFO("D-Bus Interface", "com.duokan.interface.system_manager")
//
//public:
//    SystemManagerAdaptor(QApplication &app);
//    ~SystemManagerAdaptor();
//
//public Q_SLOTS:
//
//
//Q_SIGNALS:
//
//private Q_SLOTS:
//
//private:
//
//private:
//    QString cookie_;
//};

#endif // SYSTEM_MANAGER_ADAPTOR_H_

