#ifndef SYSTEM_MANAGER_ADAPTOR_H_
#define SYSTEM_MANAGER_ADAPTOR_H_

#include <QtGui/QtGui>
//#include <QtDBus/QtDBus>

class KindleTS;
class KindleKeyboard;
class KindleFiveWay;
class SystemManager : public QObject
{
    Q_OBJECT
    SystemManager();
public:
    virtual ~SystemManager();
    static SystemManager* instance();

    void setupKindleTSMouseHandler();
    void setupKindleKeyboardHandler();
    void setupKindleFiveWayHandler();
    void suspendTouch();
    void resumeTouch();
    void enterMouseMode();
    void enterKeypadMode();
    bool isKeypadMode();

    void setFullUpdateEvery(int n) ;
    void forceFullUpdate(bool fullScreen = false);
    void setFastUpdate(bool fast = true);

public Q_SLOTS:
    //void snapshot(const QString &path);
    //bool setGrayScale(int colors);
    //int grayScale();

private:
    void loadDrivers();
    void releaseDrivers();

private:
    //ScreenManager screen_manager_; // Disable screen manager because we are using driver drawing now
    KindleTS* kindle_ts_;
    KindleKeyboard* kindle_keyboard_;
    KindleFiveWay* kindle_fiveway_;
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

