#include <QMouseDriverPlugin>
#include <QWSMouseHandler>

#include "kindlets.h"

QT_BEGIN_NAMESPACE

class TSPlugin :
#ifdef BUILD_FOR_ARM
    public QMouseDriverPlugin
#else
    public QObject
#endif
{
    Q_OBJECT
public:
    TSPlugin(QObject* parent = 0);

    QStringList keys() const;
    QWSMouseHandler* create(const QString & key, const QString & device);
};

TSPlugin::TSPlugin(QObject* parent)
#ifdef BUILD_FOR_ARM
    : QMouseDriverPlugin(parent)
#else
    : QObject(parent)
#endif
{
    qDebug("TSPlugin()");
}

QStringList TSPlugin::keys() const
{
    QStringList list;
    list << QLatin1String("KindleTS");
    return list;
}

QWSMouseHandler* TSPlugin::create(const QString & key, const QString & device)
{
    if (key.toLower() == QLatin1String("kindlets"))
    {
        qDebug("TSPlugin::create() found!");
#ifdef BUILD_FOR_ARM
        return new KindleTS(key, device);
#endif
    }

    return 0;
}

#ifndef USE_BUILT_IN_DRIVER
Q_EXPORT_PLUGIN2(KindleTS, TSPlugin)
#endif

QT_END_NAMESPACE
