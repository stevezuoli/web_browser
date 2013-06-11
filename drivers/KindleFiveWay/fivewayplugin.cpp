// fivewayplugin.cpp
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

#include "fivewayplugin.h"
#include "kindlefiveway.h"


FiveWayPlugin::FiveWayPlugin(QObject* parent)
#ifdef BUILD_FOR_ARM
    : QMouseDriverPlugin(parent)
#else
    : QObject(parent)
#endif
{
    //    qDebug("FiveWayPlugin()");
}

QStringList FiveWayPlugin::keys() const
{
    QStringList list;
    list << QLatin1String("KindleFiveWay");
    return list;
}

QWSMouseHandler* FiveWayPlugin::create(const QString & key, const QString & device)
{
    if (key.toLower() == QLatin1String("kindlefiveway"))
    {
        if (device.contains("debug", Qt::CaseInsensitive))
            qDebug("FiveWayPlugin::create() found!");
#ifdef BUILD_FOR_ARM
        return new KindleFiveWay(key, device);
#endif
    }

    return 0;
}

#ifndef USE_BUILT_IN_DRIVER
Q_EXPORT_PLUGIN2(KindleFiveWay, FiveWayPlugin)
#endif
