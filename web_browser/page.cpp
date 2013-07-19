#include "NetworkService/ns_utils.h"
#include "page.h"
#include "web_application.h"

using namespace network_service;

namespace webbrowser
{

static const QString UA_GALAXY_NEXUS = "Mozilla/5.0 (Linux; U; Android 4.0.2; en-us; Galaxy Nexus Build/ICL53F) AppleWebKit/534.30 (KHTML, like Gecko) Version/4.0 Mobile Safari/534.30";
static const QString UA_CHROME_MOBILE = "Mozilla/5.0 (Linux; Android 4.0.4; Galaxy Nexus Build/IMM76B) AppleWebKit/535.19 (KHTML, like Gecko) Chrome/18.0.1025.133 Mobile Safari/535.19";
static const QString UA_OPERA = "Opera/12.02 (Android 4.1; Linux; Opera Mobi/ADR-1111101157; U; en-US) Presto/2.9.201 Version/12.02";
static const QString UA_IOS5 = "Mozilla/5.0 (iPhone; CPU iPhone OS 5_0 like Mac OS X) AppleWebKit/534.46 (KHTML, like Gecko) Version/5.1 Mobile/9A334 Safari/7534.48.3";

WebPage::WebPage(QObject *parent)
    : QWebPage(parent)
    , network_error_(QNetworkReply::NoError)
{
    mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
    mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
    setNetworkAccessManager(WebApplication::accessManager());
    connect(this, SIGNAL(unsupportedContent(QNetworkReply *)),
            this, SLOT(handleUnsupportedContent(QNetworkReply *)));
}

bool WebPage::acceptNavigationRequest(QWebFrame *frame, const QNetworkRequest &request, NavigationType type)
{
    resetNetworkError();
    if (frame == mainFrame())
    {
        loading_url_ = request.url();
        emit loadingUrl(loading_url_);
    }

    if (type == NavigationTypeFormSubmitted || type == NavigationTypeFormResubmitted)
    {
        QWebFrame * current_frame = currentFrame();
        QWebFrame * dst_frame = (frame != 0) ? frame : current_frame;
        if (dst_frame != 0)
        {
            QSettings settings;
            settings.beginGroup(QLatin1String("websettings"));
            if ( settings.value(QLatin1String("savePasswords"), true).toBool())
            {
                QUrl url = request.url();
                if (!request.rawHeader("Referer").isEmpty())
                {
                    url = QUrl(request.rawHeader("Referer"));
                }
                //AutoComplete::instance()->setFormHtml(url, dst_frame->toHtml());
            }
            settings.endGroup();
        }

        if (current_frame != 0)
        {
            QSettings settings;
            settings.beginGroup(QLatin1String("websettings"));
            if ( settings.value(QLatin1String("savePasswords"), true).toBool())
            {
                QUrl url = current_frame->url();
                //AutoComplete::instance()->setFormHtml(url, current_frame->toHtml());
            }
            settings.endGroup();
        }
    }
    return QWebPage::acceptNavigationRequest(frame, request, type);
}

void WebPage::displayHtml(const QString & html)
{
    QList<QWebFrame*> frames;
    frames.append(mainFrame());
    while (!frames.isEmpty())
    {
        QWebFrame *frame = frames.takeFirst();
        QList<QWebFrame *> children = frame->childFrames();
        foreach(QWebFrame *frame, children)
        {
            frames.append(frame);
        }
    }
    mainFrame()->setHtml(html);
}

void WebPage::displayConnectingHtml(const QUrl &url)
{
    QString host = url.host();

    // Read file from $HOME/web_browser
    QString path;
#ifdef WIN32
    path = QDir::home().absoluteFilePath("web_browser/connecting.html");
#else
    QDir dir(getWebBrowserStoragePath());
    path = dir.absoluteFilePath("connecting.html");
#endif

    QFile file(path);
    if (file.open(QIODevice::ReadOnly))
    {
        QString title = QApplication::tr("Connecting");
        QString html = QString(QLatin1String(file.readAll())).arg(host).arg(title);
        displayHtml(html);
    }
}

void WebPage::handleUnsupportedContent(QNetworkReply *reply)
{
    qDebug("Network Reply Error:%d", reply->error());
    network_error_ = reply->error();
    if (reply->error() == QNetworkReply::NoError)
    {
        return;
    }

    // Read file from $HOME/web_browser
    QString path;
#ifdef WIN32
    path = QDir::home().absoluteFilePath("web_browser/load_fail.html");
#else
    QDir dir(getWebBrowserStoragePath());
    dir.cd("web_browser");
    path = dir.absoluteFilePath("load_fail.html");
#endif

    QFile file(path);
    if (file.open(QIODevice::ReadOnly))
    {
        QString title = tr("%1").arg(reply->url().toString());
        QString html = QString(QLatin1String(file.readAll()))
                               .arg(title)
                               .arg(reply->errorString())
                               .arg(reply->url().toString());

        QList<QWebFrame*> frames;
        frames.append(mainFrame());
        while (!frames.isEmpty())
        {
            QWebFrame *frame = frames.takeFirst();
            if (frame->url() == reply->url())
            {
                frame->setHtml(html, reply->url());
                return;
            }
            QList<QWebFrame *> children = frame->childFrames();
            foreach(QWebFrame *frame, children)
            {
                frames.append(frame);
            }
        }
        if (loading_url_ == reply->url())
        {
            mainFrame()->setHtml(html, reply->url());
        }
    }
}

QString WebPage::userAgentForUrl ( const QUrl & url ) const
{
    QString url_str = url.toEncoded();
    QString agent = UA_CHROME_MOBILE;
    //agent = QString("Mozilla/5.0+(QtEmbedded;+U;+Linux;+ru-RU;+ID=%1;+V=%2;+SID=%3)+AppleWebKit/527+(KHTML,+like+Gecko,+Safari/419.3)+GALAXY+web_browser");
    //agent = agent.arg(QUuid::createUuid().toString()).arg("1.0").arg("101010101111111");
    if (url_str.contains("baidu.com"))
    {
        agent = UA_OPERA;
    }
    else if (url_str.contains("qidian.com"))
    {
        agent = UA_IOS5;
    }
    return agent;

}

}
