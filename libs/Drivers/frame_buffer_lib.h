#ifndef _FB_LIB_H
#define _FB_LIB_H

#include <QtGui/QtGui>
#include "Screen/screen_proxy.h"

using namespace duokan::screen;

class Fblib
{
public:
    Fblib();
    ~Fblib();

    void* mmap(unsigned long * size);
    void blit(const QRect& area, const QImage& srcImg, int isX, int isY, ScreenProxy::Waveform paintFlag);
    void flash(const QRect& area, ScreenProxy::Waveform paintFlag);

private:
    bool updateScreenInfo();
    void blit4BppFast(const QRect& area, const QImage& srcImg, int isX, int isY);
	void blit8BppFast(const QRect& area, const QImage& srcImg, int isX, int isY);

    void flashKindleTouch(const QRect& area, ScreenProxy::Waveform paintFlag);
    void flashKindleTouch510(const QRect& area, ScreenProxy::Waveform paintFlag);
    void flashKindlePaperwhite(const QRect& area, ScreenProxy::Waveform paintFlag);

    QRect rect() { return QRect(0, 0, width_, height_); }

private:
    int width_;
    int height_;
    int virtual_width_;
    int virtual_height_;

    qint8 *addr_;
    qint32 addr_length_;
    qint32 fb_;
};

#endif
