#ifndef WEB_BROWSER_APPLICATION_H_
#define WEB_BROWSER_APPLICATION_H_

#ifndef _WIN32
#pragma GCC system_header
#endif

//#include <QtDBus/QtDBus>

#include "frame.h"
#include "bookmark_model.h"

namespace webbrowser
{

class WebApplication : public QApplication
{
    Q_OBJECT;

public:
    WebApplication(int &argc, char **argv);
    ~WebApplication(void);

public Q_SLOTS:
    bool open(const QString & path_name);
    bool close(const QString & path_name);
    void suspend();

    void configNetwork();
    void scan();
    void connectTo(const QString &ssid, const QString &psk);
    void onWakeUp();

private:
    void initTheme();
    void loadExternalFonts();
    void loadSettings();

private:
    BrowserFrame* main_window_;
    BookmarkModel* bookmark_model_;
};

/// Please place all dbus related sutff in this class.
/// TODO: Put it into baseframework so that all viewers can
/// use it more easily.
//class WebApplicationAdaptor : public QDBusAbstractAdaptor
//{
//    Q_OBJECT;
//    Q_CLASSINFO("D-Bus Interface", "com.duokan.interface.webbrowser");
//
//public:
//    WebApplicationAdaptor(WebApplication *application)
//        : QDBusAbstractAdaptor(application)
//        , app_(application)
//    {
//        QDBusConnection::systemBus().registerService("com.duokan.service.webbrowser");
//        QDBusConnection::systemBus().registerObject("/com/duokan/object/webbrowser", app_);
//    }
//
//public Q_SLOTS:
//    bool open(const QString & path) { return app_->open(path); }
//    bool close(const QString & path) { return app_->close(path); }
//    void suspend() { return app_->suspend(); }
//
//    void configNetwork() { app_->configNetwork(); }
//    void scan() { app_->scan(); }
//    void connectTo(const QString &ssid, const QString &psk) { app_->connectTo(ssid, psk); }
//
//private:
//    WebApplication *app_;
//};

}  // namespace webbrowser


#endif
