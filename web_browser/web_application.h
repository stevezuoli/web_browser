#ifndef WEB_BROWSER_APPLICATION_H_
#define WEB_BROWSER_APPLICATION_H_

#ifndef _WIN32
#pragma GCC system_header
#endif

#include <QApplication>
#include "NetworkService/access_manager.h"

using namespace network_service;

namespace webbrowser
{

class BrowserMainWindow;
class BookmarkModel;
class WebApplication : public QApplication
{
    Q_OBJECT;

public:
    WebApplication(int &argc, char **argv);
    ~WebApplication(void);

    static WebApplication* instance();
    static NetworkAccessManager * accessManager();

    QIcon icon(const QUrl& url) const;
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
    BrowserMainWindow* main_window_;
    BookmarkModel* bookmark_model_;

    mutable QIcon default_icon_;
};
}  // namespace webbrowser


#endif
