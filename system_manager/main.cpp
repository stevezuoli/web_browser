// -*- mode: c++; c-basic-offset: 4; -*-

#include <QtGui/QApplication>
#include <QString>
#include <QtGui/qscreen_qws.h>
#include <QtGui/qwsdisplay_qws.h>

#include "System/inc/system_manager.h"


int main(int argc, char* argv[])
{
    // We have to call this before Qt use the tslib plugin.
    // Otherwise, tslib can not initialize the touch screen.
    QApplication app(argc, argv);
    //SystemManagerAdaptor adaptor(app);

    QScreen *instance = QScreen::instance();
    const uchar *data = instance->base();
    qDebug("instance->base:%p", data);
    int degree = 0;
    degree = instance->transformOrientation() * 90;
    int width = instance->width();
    int height = instance->height();
    if (degree == 90 || degree == 270)
    {
        std::swap(width, height);
    }
    qDebug("width:%d, height:%d, pixelFormat:%d", width, height, instance->pixelFormat());
    QImage image(data, width, height, instance->pixelFormat());

    return app.exec();
}
