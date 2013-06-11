// keyboardplugin.cpp
//
// Copyright (C) 2010, 2009 Griffin I'Net, Inc. (blog@griffin.net)
//
// This file is licensed under the LGPL version 2.1, the text of which should
// be in the LICENSE.txt file, or alternately at this location:
// http://www.gnu.org/licenses/old-licenses/lgpl-2.1.txt
//
// DISCLAIMER: This software is released AS-IS with ABSOLUTELY NO WARRANTY OF
// ANY KIND.  The use of this software indicates your knowledge and acceptance
// of this.

#include <QtCore/QDebug>
#include <QtCore/QLibraryInfo>
#include <stdlib.h>

#include "keyboardplugin.h"
#include "kindlekeyboard.h"
#include "keymap/keymap.h"

KeyboardPlugin::KeyboardPlugin(QObject* parent)
#ifdef BUILD_FOR_ARM
    : QKbdDriverPlugin(parent)
#else
    : QObject(parent)
#endif
{
    //    qDebug("KeyboardPlugin()");
}

QStringList KeyboardPlugin::keys() const
{
    QStringList list;
    list << QLatin1String("KindleKeyboard");
    return list;
}

QWSKeyboardHandler* KeyboardPlugin::create(const QString & key, const QString & device)
{
    QString qs ;
    char *pth = getenv("QT_PLUGIN_PATH") ;

    if (key.toLower() == QLatin1String("kindlekeyboard"))
    {
        if (device.contains("debug", Qt::CaseInsensitive))
            qDebug("KeyboardPlugin::create() found!");

        if (pth != NULL)
            keymap_init(pth) ;
        else
        {
            qs = QLibraryInfo::location(QLibraryInfo::PluginsPath) ;
            keymap_init(qs.toAscii().data()) ;
        }
#ifdef BUILD_FOR_ARM
        return new KindleKeyboard(key, device);
#endif
    }

    return 0;
}

#ifndef USE_BUILT_IN_DRIVER
Q_EXPORT_PLUGIN2(KindleKeyboard, KeyboardPlugin)
#endif