#include "frame_buffer_lib.h"
#include "frame_buffer_ioctl.h"

#ifdef BUILD_FOR_ARM
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>
//#include <pthread.h>
//#include <semaphore.h>
//#include <signal.h>
//#include <stdarg.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <termios.h>

#include <linux/fb.h>
//#include <linux/kd.h>
//#include <linux/vt.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#endif

#include "DeviceInfo.h"
#include "interface.h"

using namespace std;

Fblib::Fblib()
    : addr_(NULL), addr_length_(0)
{
#ifdef BUILD_FOR_ARM

    fb_fix_screeninfo fb_fix;
    height_ = 0;
    
    fb_ = open("/dev/fb0", O_RDWR);
    if (fb_ < 0 )
    {
        qDebug( "Can not open frame buffer fb0!!!");
        goto err0;
    }
    
    if (ioctl(fb_, FBIOGET_FSCREENINFO, &fb_fix) < 0 )
    {
        goto err1;
    }

    addr_ = (qint8*)mmap(NULL, fb_fix.smem_len, PROT_READ|PROT_WRITE, MAP_SHARED, fb_, 0);
    if (NULL == addr_)
    {
        goto err1;
    }

    addr_length_ =  fb_fix.smem_len;

    if (!this->updateScreenInfo())
    {
        qDebug("Can not get screeninfo!!!");
        goto err2;
    }
    return;
err2:
    munmap(addr_, fb_fix.smem_len);
    addr_ = 0;
err1:
    close(fb_);
    fb_ = -1;
err0:
    return;
#else
    width_ = DeviceInfo::GetDisplayScreenWidth();
    height_ = DeviceInfo::GetDisplayScreenHeight();
    virtual_width_ = width_;
    virtual_height_ = height_;
    const char EMULATOR_FB_MAP[] = "/DuoKan/fb_shmem";
#ifndef WIN32
    fb_ = open(EMULATOR_FB_MAP, O_RDWR | O_CREAT | O_TRUNC, 0666);
    if (fb_ >= 0)
    {
        lseek(fb_, width_ * height_, SEEK_SET);
        write(fb_, &fb_, 1);
        struct stat statbuf;
        int flags = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP;
        stat(EMULATOR_FB_MAP, &statbuf);
        chmod(EMULATOR_FB_MAP, statbuf.st_mode | flags);
        addr_ = (INT8*)mmap(NULL, width_ * height_, PROT_READ | PROT_WRITE, MAP_SHARED, fb_, 0);
    }
#endif

#endif
}


Fblib::~Fblib()
{
#ifdef BUILD_FOR_ARM
    if(0 != addr_)
    {
        munmap(addr_,  addr_length_);
        addr_ = 0;
    }

    if(fb_ >= 0)
    {
        close(fb_);
        fb_ = -1;
    }

#endif
}

void* Fblib::mmap(unsigned long * size)
{
    if (fb < 0)
    {
        *size = -1;
        return 0;
    }
    *size = addr_length_;
    return addr_;
}


/* @ 2011.12.20 luoxuping, for Kindle 4 slim,  e-ink driver uses 8 bits per pixel */
void Fblib::blit8BppFast(const QRect& area, const QImage& srcImg, int isX, int isY)
{
    int srcStrip = srcImg.bytesPerLine();
    int bpp = 8; //  for kindle 4 slim,  dst bpp is 8
    int dstStrip = (virtual_width_ * bpp + 7)/8 ;

    const uchar* srcStartPerRow = srcImg.bits() + srcStrip * isY + isX;
    uchar* dstStartPerRow = (uchar*)addr_ + dstStrip * area.top() + area.left();
    //uchar* buffer = new uchar[iWidth];
    for (int y = 0; y < area.height(); ++y)
    {
        //if (NULL != buffer)
        //{
        //    for (int x = 0; x < iWidth; ++x)
        //    {
        //        uchar c = srcStartPerRow[x];
        //        c = c & 0xf0;
        //        buffer[x] = c;
        //    }
        //    memcpy(dstStartPerRow, buffer, iWidth);
        //}
        //else
        //{
        memcpy(dstStartPerRow, srcStartPerRow, area.width());
        //}
        srcStartPerRow += srcStrip;
        dstStartPerRow += dstStrip;
    }
    //if (NULL == buffer)
    //{
    //    delete []buffer;
    //}

	return;
}

void Fblib::blit4BppFast(const QRect& area, const QImage& srcImg, int isX, int isY)
{
    int srcStrip = srcImg.bytesPerLine();
    int bpp = 4; // here we assume dst bpp is 4
    int dstStrip = (virtual_width_ * bpp + 31) / 32 * 4;

    const uchar* srcStartPerRow = srcImg.bits() + srcStrip * isY + isX;
    uchar* dstStartPerRow = (uchar*)addr_ + dstStrip * area.top() + area.left() / 2;
    for (int y = 0; y < area.height(); ++y)
    {
        const uchar* curSrcU8 = srcStartPerRow;
        uchar* curDstU8 = dstStartPerRow;
        int iXTemp = area.left();

        if(iXTemp % 2)
        {
            *curDstU8 &= 0xF0;
            *curDstU8 |= ((*curSrcU8) & 0xF0) >> 4;
            ++iXTemp;
            ++curDstU8;
            ++curSrcU8;
        }
        if(iXTemp % 4)
        {
            *curDstU8 = ((*curSrcU8) & 0xF0) | (((*(curSrcU8 + 1)) & 0xF0) >> 4);
            ++curDstU8;
            iXTemp += 2;
            curSrcU8 += 2;
        }

        quint32 *curSrc = (quint32*)(curSrcU8);
        quint16 *curDst = (quint16*)(curDstU8);
        quint32 *srcRealEnd = (quint32*)(srcStartPerRow + area.width());
        quint32 *srcLoopEnd = (quint32*)(srcStartPerRow + area.width() - 3);
        for (;curSrc < srcLoopEnd; ++curSrc, ++curDst)
        {
            quint32 t = *curSrc;
            if (0 == t)
            {
                *curDst = 0;
            }
            else
            {
                // PENGF: assume little endian here
                // source bytes are AB CD EF GH
                // quint32 is 0xGHEFCDAB
                // related dst bytes should be AC EG
                // quint16 is 0xEGAC

                // initially t = 0xGHEFCDAB
                t &= 0xF0F0F0F0; // t = 0xG0E0C0A0
                t |= t >> 12; // t = (0xG0E0C0A0 | 0x000G0E0C) = 0xG0EGCEAC
                t &= 0x00FF00FF; // t = 0x00EG00AC
                t |= t >> 8; // t = 0x00EGEGAC
                *curDst = quint16(t & 0xFFFF); // *curDst = 0xEGAC
            }
        }
        if (curSrc < srcRealEnd)
        {
            curSrcU8 = (uchar*)curSrc;
            curDstU8 = (uchar*)curDst;
            int remainToCopy = (uchar*)srcRealEnd - (uchar*)curSrc;
            if (remainToCopy >= 2)
            {
                *curDstU8=((*curSrcU8) & 0xF0) | (((*(curSrcU8 + 1)) & 0xF0) >> 4);
                remainToCopy-=2;
                ++curDstU8;
                curSrcU8+=2;
            }
            if (remainToCopy > 0)
            {
                *curDstU8 &= 0x0F;
                *curDstU8 |= (*curSrcU8) & 0xF0;
            }
        }
        srcStartPerRow += srcStrip;
        dstStartPerRow += dstStrip;
    }
}

void Fblib::blit(const QRect& area, const QImage& srcImg, int isX, int isY, ScreenProxy::Waveform paintFlag)
{
    int iX = area.left();
    int iY = area.top();
    int iWidth = area.width();
    int iHeight = area.height();

    qDebug("Fblib::BitBlt(iX=%d, iY=%d, iWidth=%d, iHeight=%d, isX=%d, isY=%d)", iX, iY, iWidth, iHeight, isX, isY);

    //clock_t start = clock();
    if(srcImg.isNull() || NULL == addr_)
    {
        qDebug(" Fblib::BitBlt failed @ 1");
        return;
    }

    QRect targetRect = rect().intersected(QRect(iX, iY, iWidth, iHeight)); 
    if (targetRect.isEmpty())
    {
        qDebug(" Fblib::BitBlt failed @ 2");
        return;
    }

    QRect srcRect = srcImg.rect().intersected(QRect(isX, isY, iWidth, iHeight));
    if (srcRect.isEmpty())
    {
        qDebug(" Fblib::BitBlt failed @ 3");
        return;
    }

    int copy_width = std::min(srcRect.width(), targetRect.width());
    int copy_height = std::min(srcRect.height(), targetRect.height());

    QRect update_area(iX, iY, copy_width, copy_height);

//#ifdef BUILD_FOR_ARM
    if (DeviceInfo::IsK4NT() ) {
        qDebug(" Fblib::BitBlt on Kindle 4 non-touch");
		blit8BppFast(update_area, srcImg, isX, isY);
    }else if (DeviceInfo::IsK4Touch())
    {
        qDebug(" Fblib::BitBlt on Kindle 4 Touch");
		blit8BppFast(update_area, srcImg, isX, isY);
    }else if (DeviceInfo::IsKPW())
	{
        qDebug(" Fblib::BitBlt on Kindle Paperwhite");
		blit8BppFast(update_area, srcImg, isX, isY);
	}
    else    
    {
		blit4BppFast(update_area, srcImg, isX, isY);
	}

    //qDebug(DLC_TIME, "Fblib::BitBlt() Time elapsed=%d ms.",
    //        1000*(clock()-start)/CLOCKS_PER_SEC);

    if (DeviceInfo::IsK4Touch())
    {
        qDebug( " Fblib::BitBlt update screen on Kindle Touch");
		if(DeviceInfo::IsTouch510()) 
		{

			UpdateToScreenKindleTouch510(targetRect.left(), targetRect.top(), nCopyWidth, nCopyHeight, paintFlag);
		}else 
		{
			UpdateToScreenKindleTouch(targetRect.left(), targetRect.top(), nCopyWidth, nCopyHeight, paintFlag);
		}
    }
    else if (DeviceInfo::IsKPW())
	{
		UpdateToScreenKindlePaperwhite(targetRect.left(), targetRect.top(), nCopyWidth, nCopyHeight, paintFlag);
	}
	else 
	{
        UpdateToScreen(targetRect.left(), targetRect.top(), nCopyWidth, nCopyHeight, paintFlag);
	}


//#endif
}


// 猜想: the difference between FBIO_EINK_UPDATE_DISPLAY_AREA and FBIO_EINK_UPDATE_DISPLAY,
// Maybe FBIO_EINK_UPDATE_DISPLAY won't do a full pixel compare before sending to EPSON for refreshing.

void Fblib::ScreenRefresh()
{
#ifdef BUILD_FOR_ARM
    StopWatch watch(StopWatch::INIT_START);
#endif
    qDebug( "Fblib::ScreenRefresh()");
    if (DeviceInfo::IsK4Touch())
    {
        qDebug( " Fblib::BitBlt update screen on Kindle Touch");
        if(DeviceInfo::IsTouch510()) 
        {

            UpdateToScreenKindleTouch510(0, 0, GetWidth(), GetHeight(), duokan::screen::ScreenProxy::GC);
        }else 
        {
            UpdateToScreenKindleTouch(0, 0, GetWidth(), GetHeight(), duokan::screen::ScreenProxy::GC);
        }
    }
	else if (DeviceInfo::IsKPW())
	{
		UpdateToScreenKindlePaperwhite(0, 0, GetWidth(), GetHeight(), duokan::screen::ScreenProxy::GC);
	}
    else
    {
        UpdateToScreen(0, 0, GetWidth(), GetHeight(), duokan::screen::ScreenProxy::GC);
    }
    //ioctl(fb_, FBIO_EINK_RESTORE_DISPLAY, fx_update_full);
    //ioctl(fb_, FBIO_EINK_RESTORE_DISPLAY, fx_update_full);
#ifdef BUILD_FOR_ARM
    watch.Stop();
    qDebug( "Fblib::ScreenRefresh() Time elapsed=%d ms.",
      watch.DurationInMs());
    qDebug( "Fblib::ScreenRefresh() Time elapsed=%d ms.",
        watch.DurationInMs());
#endif
}


void Fblib::UpdateToScreen(INT32 iX,  INT32 iY, INT32 iWidth, INT32 iHeight, duokan::screen::ScreenProxy::Waveform paintFlag)
{
#ifdef BUILD_FOR_ARM
    StopWatch watch(StopWatch::INIT_START);
#endif
    qDebug( "Fblib::UpdateToScreen(iX=%d, iY=%d, iWidth=%d, iHeight=%d)",
        iX,  iY, iWidth, iHeight);

    struct update_area_t update_area;
    memset(&update_area,0,sizeof(update_area_t));
    update_area.x1 = iX;
    update_area.y1 = iY;
    update_area.x2 = iX + iWidth;
    update_area.y2 = iY + iHeight;
    update_area.buffer = 0;
    if(paintFlag & duokan::screen::ScreenProxy::GC)
    {
        qDebug( "Fblib::UpdateToScreen PAINT_FLAG_FULL");
        ++addr_[0]; // 如果CRC不发生改变，驱动不会刷屏
        update_area.which_fx = ( enum fx_type)fx_update_full;
#ifdef BUILD_FOR_ARM
        ioctl(fb_, FBIO_EINK_UPDATE_DISPLAY, (enum fx_type)fx_update_full);
        //ioctl(fb_, FBIO_EINK_UPDATE_DISPLAY_AREA, (unsigned long)&update_area);
#endif
    }
    else
    {
        update_area.which_fx =( enum fx_type)fx_update_partial;
#ifdef BUILD_FOR_ARM
        if (iWidth > (width_ - 50) && iHeight > (height_ - 50))
            ioctl(fb_, FBIO_EINK_UPDATE_DISPLAY, update_area.which_fx);
        else
            ioctl(fb_, FBIO_EINK_UPDATE_DISPLAY_AREA, (unsigned long)&update_area);
#endif
    }

#ifdef BUILD_FOR_ARM
    watch.Stop();
    qDebug( "Fblib::UpdateToScreen() Time elapsed=%d ms.",
        watch.DurationInMs());

    qDebug( "Fblib::UpdateToScreen() Time elapsed=%d ms.",
        watch.DurationInMs());
#endif
}


void Fblib::UpdateToScreenKindleTouch(INT32 iX,  INT32 iY, INT32 iWidth, INT32 iHeight, duokan::screen::ScreenProxy::Waveform paintFlag)
{
#ifdef BUILD_FOR_ARM
    StopWatch watch(StopWatch::INIT_START);
#endif
    qDebug( "Fblib::UpdateToScreenKindleTouch(iX=%d, iY=%d, iWidth=%d, iHeight=%d)",
        iX,  iY, iWidth, iHeight);

    mxcfb_update_data update_data;
    mxcfb_rect update_region;
    update_region.top = iY;
    update_region.left = iX;
    update_region.width = iWidth;
    update_region.height = iHeight;

    qDebug( " Fblib::UpdateToScreenKindleTouch with (%d,%d,%d,%d)",iY,iX,iWidth,iHeight);

    memset(&update_data,0,sizeof(update_data));
    update_data.update_region = update_region;
    update_data.waveform_mode = WAVEFORM_MODE_AUTO;
    update_data.temp = TEMP_USE_AMBIENT;

    update_data.flags = EPDC_FLAG_ENABLE_INVERSION;
    update_data.update_marker = 0x002a; //同步用

    update_data.alt_buffer_data.alt_update_region = update_region;
    update_data.alt_buffer_data.phys_addr = (quint32) addr_;
    update_data.alt_buffer_data.height = iHeight;
    update_data.alt_buffer_data.width = iWidth;

    if(paintFlag == duokan::screen::ScreenProxy::GC)
    {
        update_data.update_mode =   UPDATE_MODE_FULL;
		update_data.waveform_mode = WAVEFORM_MODE_GC16;
    }
    else
    {
        update_data.update_mode =   UPDATE_MODE_PARTIAL;
    }

    //ioctl(fb_, MXCFB_SET_AUTO_UPDATE_MODE, AUTO_UPDATE_MODE_AUTOMATIC_MODE);
#ifdef BUILD_FOR_ARM
    ioctl(fb_, MXCFB_SEND_UPDATE,  (unsigned long)&update_data);
#endif

    //if(paintFlag & PAINT_FLAG_FULL)
    //{
    //    qDebug(DLC_EINK_PAINT, "Fblib::UpdateToScreen PAINT_FLAG_FULL");
    //    //++addr_[0]; // Remark(zhaigh): 去掉这句话会导致手动刷屏无效，原因不确定
    //    ioctl(fb_, MXCFB_SEND_UPDATE, (unsigned long)&update_data);
    //}
    //else
    //{
    //    if (iWidth > (width_ - 50) && iHeight > (height_ - 50))
    //        ioctl(fb_, MXCFB_SEND_UPDATE, (unsigned long)&update_data);
    //    else
    //        ioctl(fb_, MXCFB_SEND_UPDATE,  (unsigned long)&update_data);
    //}
#ifdef BUILD_FOR_ARM
    watch.Stop();
    qDebug( "Fblib::UpdateToScreenKindleTouch() Time elapsed=%d ms.",
        watch.DurationInMs());

    qDebug( "Fblib::UpdateToScreenKindleTouch() Time elapsed=%d ms.",
        watch.DurationInMs());
#endif
}

// Touch v5.1.0 mxcfb_update_data结构体参数改为mxcfb_update_data_v510
void Fblib::UpdateToScreenKindleTouch510(INT32 iX,  INT32 iY, INT32 iWidth, INT32 iHeight, duokan::screen::ScreenProxy::Waveform paintFlag)
{
#ifdef BUILD_FOR_ARM
	StopWatch watch(StopWatch::INIT_START);
#endif
	qDebug("Fblib::UpdateToScreenKindleTouch510(iX=%d, iY=%d, iWidth=%d, iHeight=%d)",
		iX,  iY, iWidth, iHeight);

	mxcfb_update_data_v510 update_data;
	mxcfb_rect update_region;
	update_region.top = iY;
	update_region.left = iX;
	update_region.width = iWidth;
	update_region.height = iHeight;

	qDebug(" Fblib::UpdateToScreenKindleTouch510 with (%d,%d,%d,%d)",iY,iX,iWidth,iHeight);

	memset(&update_data,0,sizeof(update_data));
	update_data.update_region = update_region;
	update_data.waveform_mode = WAVEFORM_MODE_AUTO;
	update_data.temp = TEMP_USE_AMBIENT;

	update_data.flags = EPDC_FLAG_ENABLE_INVERSION;
	update_data.update_marker = 0x002a; //同步用

	update_data.alt_buffer_data.alt_update_region = update_region;
	update_data.alt_buffer_data.phys_addr = (quint32) addr_;
	update_data.alt_buffer_data.height = iHeight;
	update_data.alt_buffer_data.width = iWidth;

	update_data.hist_bw_waveform_mode = WAVEFORM_MODE_AUTO;
	update_data.hist_gray_waveform_mode =  WAVEFORM_MODE_AUTO;

    if(paintFlag == duokan::screen::ScreenProxy::GC)
	{
		update_data.update_mode =   UPDATE_MODE_FULL;
		update_data.waveform_mode = WAVEFORM_MODE_GC16;
	}
	else
	{
		update_data.update_mode =   UPDATE_MODE_PARTIAL;
	}

#ifdef BUILD_FOR_ARM
	ioctl(fb_, MXCFB_SEND_UPDATE_V510,  (unsigned long)&update_data);
	watch.Stop();
	qDebug("Fblib::UpdateToScreenKindleTouch510() Time elapsed=%d ms.",
		watch.DurationInMs());

	qDebug("Fblib::UpdateToScreenKindleTouch510() Time elapsed=%d ms.",
		watch.DurationInMs());
#endif
}

void Fblib::UpdateToScreenKindlePaperwhite(INT32 iX,  INT32 iY, INT32 iWidth, INT32 iHeight, duokan::screen::ScreenProxy::Waveform paintFlag)
{
	UpdateToScreenKindleTouch510(iX, iY, iWidth, iHeight, paintFlag);
}

bool Fblib::SetOrientation(orientation_t orientation)
{
    return UpdateHardwareScreenInfo();
}

bool Fblib::GetOrientation(orientation_t* orientation) const
{
    if (NULL == orientation)
    {
        return false;
    }

#ifdef BUILD_FOR_ARM
    if (ioctl(fb_, FBIO_EINK_GET_DISPLAY_ORIENTATION, orientation))
    {
        qWarning( "Set orientation failed\n");
        return false;
    }
#endif
    return true;
}

bool Fblib::SetRebootBehavior(reboot_behavior_t reboot_behavior)
{
#ifdef BUILD_FOR_ARM
    if (ioctl(fb_, FBIO_EINK_SET_REBOOT_BEHAVIOR, reboot_behavior))
    {
        qWarning( "Fblib::SetRebootBehavior() failed\n");
        return false;
    }
#endif
    return true;
}

bool Fblib::GetRebootBehavior(reboot_behavior_t* reboot_behavior) const
{
    if (NULL == reboot_behavior)
    {
        return false;
    }

#ifdef BUILD_FOR_ARM
    if (ioctl(fb_, FBIO_EINK_GET_REBOOT_BEHAVIOR, reboot_behavior))
    {
        qWarning( "Fblib::GetRebootBehavior() failed");
        return false;
    }
#endif
    return true;
}

bool Fblib::UpdateHardwareScreenInfo()
{
#ifdef BUILD_FOR_ARM
    fb_var_screeninfo fb_var;
    if(ioctl(fb_, FBIOGET_VSCREENINFO, &fb_var) == -1)
    {
        qDebug("FBIOGET_VSCREENINFO can't open!!!");
        return false;
    }
    width_ = fb_var.xres;
    height_ = fb_var.yres;

    virtual_width_ = fb_var.xres_virtual;
    virtual_height_ = fb_var.yres_virtual;

    qDebug("framebuffer with size %d x %d", width_, height_);
    qDebug("framebuffer with virtual size %d x %d", virtual_width_, virtual_height_);
#else
    width_ = DeviceInfo::GetDisplayScreenWidth();
    height_ = DeviceInfo::GetDisplayScreenHeight();
    virtual_width_ = width_;
    virtual_height_ = height_;
#endif
    return true;
}
