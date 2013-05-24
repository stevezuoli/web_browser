#include <QMouseDriverPlugin>
#include <QWSMouseHandler>

#include "kindlets.h"

QT_BEGIN_NAMESPACE

class TSPlugin : public QMouseDriverPlugin {
    Q_OBJECT
public:
    TSPlugin(QObject* parent = 0);

    QStringList keys() const;
    QWSMouseHandler* create(const QString & key, const QString & device);
};

TSPlugin::TSPlugin(QObject* parent) : QMouseDriverPlugin(parent)
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
        return new KindleTS(key, device);
    }

    return 0;
}

Q_EXPORT_PLUGIN2(KindleTS, TSPlugin)

QT_END_NAMESPACE
