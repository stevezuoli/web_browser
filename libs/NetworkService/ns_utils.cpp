#include "ns_utils.h"

namespace network_service
{

static const QString CACHE_DIR = ".web_cache";
static const QString DOWNLOAD_FILE_DIR = "downloads";
static const QString WEB_STORE_PATH = "/mnt/us/DK_System/xKindle/web_browser/";

QString getWebBrowserStoragePath()
{
    return WEB_STORE_PATH;
}

QString getCacheLocation()
{
    QString path = getWebBrowserStoragePath();
    QDir dir(path);
    QString ret;
    if (!dir.exists(CACHE_DIR))
    {
        if (!dir.mkdir(CACHE_DIR))
        {
            return ret;
        }
    }

    if (dir.cd(CACHE_DIR))
    {
        ret = dir.absolutePath();
    }
    return ret;
}

QUrl guessUrlFromString(const QString &string)
{
    QString url_str = string.trimmed();
    QRegExp test(QLatin1String("^[a-zA-Z]+\\:.*"));

    // Check if it looks like a qualified URL. Try parsing it and see.
    bool has_schema = test.exactMatch(url_str);
    if (has_schema)
    {
        QUrl url = QUrl::fromEncoded(url_str.toUtf8(), QUrl::TolerantMode);
        if (url.isValid())
        {
            return url;
        }
    }

    // Might be a file.
    if (QFile::exists(url_str))
    {
        QFileInfo info(url_str);
        return QUrl::fromLocalFile(info.absoluteFilePath());
    }

    // Might be a shorturl - try to detect the schema.
    if (!has_schema)
    {
        int dot_index = url_str.indexOf(QLatin1Char('.'));
        if (dot_index != -1)
        {
            QString prefix = url_str.left(dot_index).toLower();
            QByteArray schema = (prefix == QLatin1String("ftp")) ? prefix.toLatin1() : "http";
            QUrl url = QUrl::fromEncoded(schema + "://" + url_str.toUtf8(), QUrl::TolerantMode);
            if (url.isValid())
            {
                return url;
            }
        }
    }

    // Fall back to QUrl's own tolerant parser.
    QUrl url = QUrl::fromEncoded(string.toUtf8(), QUrl::TolerantMode);

    // finally for cases where the user just types in a hostname add http
    if (url.scheme().isEmpty())
    {
        url = QUrl::fromEncoded("http://" + string.toUtf8(), QUrl::TolerantMode);
    }
    return url;
}

static char IntToHexChar(int i)
{
    if (0 <= i && i <= 9)
    {
        return i + '0';
    }
    else if (i < 16)
    {
        return i - 10 + 'A';
    }
    else
    {
        return 0;
    }
}

static int HexCharToInt(char c)
{
    if ('0' <= c && c <= '9')
    {
        return c - '0';
    }
    else if ('a' <= c && c <= 'f')
    {
        return 10 + c - 'a';
    }
    else if ('A' <= c && c <= 'F')
    {
        return 10 + c - 'A';
    }
    return -1;
}

QString UrlEncode(const char* s)
{
    if (NULL == s)
    {
        return "";
    }
    const unsigned char* us = (const unsigned char*)s;
    QString result;
    while (unsigned int c = *us++)
    {
        if (isalnum(c))
        {
            result.push_back(c);
        }
        else if (' ' == c)
        {
            result.push_back('+');
        }
        else
        {
            result.push_back('%');
            result.push_back(IntToHexChar(c / 16));
            result.push_back(IntToHexChar(c % 16));
        }
    }
    return result;
}

QString UrlDecode(const char* s)
{
    if (NULL == s)
    {
        return "";
    }
    const char* sEnd = s + strlen(s);
    QString result;
    while (s != sEnd)
    {
        if ('+' == *s)
        {
            result.push_back(' ');
            ++s;
        }
        else if ('%' == *s && s + 2 <= sEnd)
        {
            int v1 = HexCharToInt(*(s+1));
            int v2 = HexCharToInt(*(s+2));
            if (v1 < 0 || v2 < 0)
            {
                return result;
            }
            result.push_back(16 * v1 + v2);
            s += 3;
        }
        else
        {
            result.push_back(*s);
            ++s;
        }
    }
    return result;
}

std::string UrlEncodeForStdString(const char* s)
{
    if (NULL == s)
    {
        return "";
    }
    const unsigned char* us = (const unsigned char*)s;
    std::string result;
    while (unsigned int c = *us++)
    {
        if (isalnum(c))
        {
            result.push_back(c);
        }
        else if (' ' == c)
        {
            result.push_back('+');
        }
        else
        {
            result.push_back('%');
            result.push_back(IntToHexChar(c / 16));
            result.push_back(IntToHexChar(c % 16));
        }
    }
    return result;
}

}
