#ifndef NETWORK_SERVICE_UTILS
#define NETWORK_SERVICE_UTILS

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QAuthenticator>
#include <QtNetwork/QNetworkDiskCache>
#include <QtNetwork/QNetworkProxy>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QSslError>
#include <QUrl>
#include <QMap>
#include <QDomElement>
#include <QSettings>
#include <QWebFrame>
#include <QWebSettings>
#include <QtGui/QDesktopServices>
#include <QtCore/QSettings>

#include <QtGui/QtGui>
#include <QtWebKit/QtWebKit>

namespace network_service
{

QString getCacheLocation();
QString getDownloadFileLocation();
QUrl guessUrlFromString(const QString &string);

};

#endif
