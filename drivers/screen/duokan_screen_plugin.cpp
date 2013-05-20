#include <qscreendriverplugin_qws.h>
#include <qstringlist.h>
#include "duokan_screen.h"

QT_BEGIN_NAMESPACE

class DuokanScreenPlugin : public QScreenDriverPlugin
{
public:
    DuokanScreenPlugin();

    QStringList keys() const;
    QScreen *create(const QString&, int displayId);
};

DuokanScreenPlugin::DuokanScreenPlugin()
    : QScreenDriverPlugin()
{
}

QStringList DuokanScreenPlugin::keys() const
{
    QStringList list;
    list << QLatin1String("DuokanScreen");
    return list;
}

QScreen* DuokanScreenPlugin::create(const QString& driver, int displayId)
{
    if (driver.compare("duokanscreen", Qt::CaseInsensitive) == 0)
    {
        return new DuokanScreen(displayId);
    }

    return 0;
}

Q_EXPORT_PLUGIN2(DuokanScreen, DuokanScreenPlugin)

QT_END_NAMESPACE


