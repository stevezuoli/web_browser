#include "web_application.h"

using namespace network_service;

namespace webbrowser
{

static const int CACHE_MIN_DEAD_CAPACITY = 1024 * 1024;
static const int CACHE_MAX_DEAD = 3 * 1024 * 1024;
static const int TOTAL_CAPACITY = 4 * 1024 * 1024;

WebApplication::WebApplication(int &argc, char **argv)
    : QApplication(argc, argv)
    , main_window_(new BrowserFrame())
    , bookmark_model_(new BookmarkModel())
{
    QCoreApplication::setOrganizationName(QLatin1String("Duokan"));
    QCoreApplication::setApplicationName(QLatin1String("Web Browser"));

    QWebSettings::setMaximumPagesInCache(4);
    QWebSettings::setObjectCacheCapacities(CACHE_MIN_DEAD_CAPACITY, CACHE_MAX_DEAD, TOTAL_CAPACITY);

    //main_window_->attachBookmarkModel(bookmark_model_.get());
    initTheme();
    loadExternalFonts();
    loadSettings();
}

WebApplication::~WebApplication(void)
{
}

bool WebApplication::open(const QString & path_name)
{
    if (main_window_ == 0)
    {
        return false;
    }

    main_window_->load(path_name);
    main_window_->show();
    return true;
}

bool WebApplication::close(const QString & path)
{
    return true;
}

void WebApplication::suspend()
{
    // Do not stop wpa supplicant because explorer has done so.
    //qDebug("About to suspend. Stop wpa supplicant!");
    //sys::SysStatus::instance().stopWpaSupplicant();
}

void WebApplication::onWakeUp()
{
    qDebug("Wake Up! Configure the network now.");
    configNetwork();
}

void WebApplication::configNetwork()
{
}

void WebApplication::scan()
{
}

void WebApplication::connectTo(const QString &ssid, const QString &psk)
{
}

NetworkAccessManager* WebApplication::accessManager()
{
    return network_service::getAccessManagerInstance();
}

void WebApplication::initTheme()
{
    // We change the active selection background color so that
    // all viewers have the same look and feel.
    // Have to use the 254 as alpha value, as the webkit itself does blend
    // with white color. When using 16 grey level color, the result is same
    // as complete black.
    QPalette p(QApplication::palette());
    QBrush b(QColor(0, 0, 0, 254));
    p.setBrush(QPalette::Active, QPalette::Highlight, b);
    p.setBrush(QPalette::Inactive, QPalette::Highlight, b);

    b.setColor(QColor(255, 255, 255, 254));
    p.setBrush(QPalette::Active, QPalette::HighlightedText, b);
    p.setBrush(QPalette::Inactive, QPalette::HighlightedText, b);

    QApplication::setPalette(p);
}

void WebApplication::loadExternalFonts()
{
    // Before we open the document, make sure the external fonts
    // have been installed. It's necessary as user may use external
    // fonts, but by default, these fonts are not loaded.
}

void WebApplication::loadSettings()
{
    QSettings settings;
    settings.beginGroup(QLatin1String("websettings"));

    QWebSettings *default_settings = QWebSettings::globalSettings();
    default_settings->setAttribute(QWebSettings::JavascriptEnabled, true);
    default_settings->setAttribute(QWebSettings::PluginsEnabled, true);
    default_settings->setAttribute(QWebSettings::AutoLoadImages, settings.value(QLatin1String("autoLoadImages"), true).toBool());
    default_settings->setFontSize(QWebSettings::DefaultFontSize, 20);
    settings.endGroup();
}

} // namespace webbrowser


