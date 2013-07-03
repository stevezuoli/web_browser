#include "web_application.h"
#include "browser_mainwindow.h"
#include "bookmark_model.h"
#include "const_strings.h"
#include "common/ImageManager.h"
#include "common/WindowsMetrics.h"

using namespace network_service;
using namespace ui;
using namespace ui::windowsmetrics;

namespace webbrowser
{

static const int CACHE_MIN_DEAD_CAPACITY = 1024 * 1024;
static const int CACHE_MAX_DEAD = 3 * 1024 * 1024;
static const int TOTAL_CAPACITY = 4 * 1024 * 1024;

QString WebApplication::localStoragePath()
{
#ifdef BUILD_FOR_ARM
    return "mnt/us/DK_System/xKindle/web_browser";
#else
    return QDir::homePath();
#endif
}

WebApplication* WebApplication::instance()
{
    return static_cast<WebApplication*>(QCoreApplication::instance());
}

WebApplication::WebApplication(int &argc, char **argv)
    : QApplication(argc, argv)
{
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTranslator* translator = new QTranslator();
    translator->load(":/res/WebBrowser_Chn");
    installTranslator(translator);

    main_window_ = new BrowserMainWindow();
    bookmark_model_ = new BookmarkModel();

    QCoreApplication::setOrganizationName(QLatin1String("Duokan"));
    QCoreApplication::setApplicationName(QLatin1String("Web Browser"));
    customizeBrwoser();

    // parse arguments
    if (argc >= 2)
    {
        init_url_path_ = QString::fromLocal8Bit(argv[1]);
    }

    if (argc >= 3)
    {
        additional_option_ = QString::fromLocal8Bit(argv[2]);
    }

    //main_window_->attachBookmarkModel(bookmark_model_.get());
    initTheme();
    loadExternalFonts();
    loadSettings();
}

WebApplication::~WebApplication(void)
{
}

bool WebApplication::open()
{
    if (main_window_ == 0)
    {
        return false;
    }

    main_window_->load(init_url_path_, additional_option_);
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
    
void WebApplication::customizeBrwoser()
{
    QWebSettings::setMaximumPagesInCache(4);
    QWebSettings::setObjectCacheCapacities(CACHE_MIN_DEAD_CAPACITY, CACHE_MAX_DEAD, TOTAL_CAPACITY);
    QWebSettings::enablePersistentStorage(localStoragePath());
    
    QWebSettings* global_settings = QWebSettings::globalSettings();
    global_settings->setAttribute(QWebSettings::JavascriptCanOpenWindows, true);
    global_settings->setAttribute(QWebSettings::JavascriptCanCloseWindows, true);
    global_settings->setAttribute(QWebSettings::JavascriptCanAccessClipboard, true);
    global_settings->setAttribute(QWebSettings::FrameFlatteningEnabled, true);
    global_settings->setAttribute(QWebSettings::LocalStorageEnabled, true);
    QString dirName = QString(ConstStrings::WEBBROWSER_RES_PATH) + QLatin1String("/.") + QString(ConstStrings::FAVICONS);
    QDir dir(dirName);
    if (!dir.exists())
    {
        dir.mkpath(dirName);
    }
    QWebSettings::setIconDatabasePath(dirName);
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

QIcon WebApplication::icon(const QUrl& url) const
{
    QIcon icon = QWebSettings::iconForUrl(url);
    if (!icon.isNull())
    {
        return icon.pixmap(GetWindowMetrics(UrlFaviconSizeIndex),GetWindowMetrics(UrlFaviconSizeIndex));
    }
    if (default_icon_.isNull())
        default_icon_ = QIcon(ImageManager::GetImagePath(IMAGE_BROWSER));

    return default_icon_.pixmap(GetWindowMetrics(UrlFaviconSizeIndex),GetWindowMetrics(UrlFaviconSizeIndex));
}
} // namespace webbrowser


