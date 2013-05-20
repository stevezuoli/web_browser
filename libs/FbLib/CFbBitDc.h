////////////////////////////////////////////////////////////////////////
// CFbBitDc.h
// Contact:
// Copyright (c) Duokan Corporation. All rights reserved.
//
//
////////////////////////////////////////////////////////////////////////

#ifndef __CFBBITDC_H__
#define __CFBBITDC_H__

#include <QtGui/QtGui>
#include "Screen/screen_proxy.h"

using namespace duokan;
using namespace duokan::screen;

enum orientation_t
{
    orientation_portrait,
    orientation_portrait_upside_down,
    orientation_landscape,
    orientation_landscape_upside_down
};
typedef enum orientation_t orientation_t;

enum reboot_behavior_t
{
    reboot_screen_asis,
    reboot_screen_clear,
    reboot_screen_splash
};

typedef enum reboot_behavior_t reboot_behavior_t;


class CFbBitDc
{
public:
    CFbBitDc();

    ~CFbBitDc();

    //重新刷整个屏幕
    void ScreenRefresh();
    //将 DK_IMAGE 数据刷到屏幕
    void BitBlt(qint32 iX, qint32 iY, qint32 iWidth, qint32 iHeight,
        const QImage& srcImg, qint32 isX, qint32 isY,  duokan::screen::ScreenProxy::Waveform paintFlag);

    //物理屏幕宽度
    qint32 GetWidth() const
    {
        return m_nWidth;
    }

    //物理屏幕高度
    qint32 GetHeight() const
    {
        return m_nHeight;
    }

    bool SetOrientation(orientation_t orientation);
    bool GetOrientation(orientation_t* orientation) const;

private:
    //没有用的 api
    bool SetRebootBehavior(reboot_behavior_t reboot_behavior);
    bool GetRebootBehavior(reboot_behavior_t* reboot_behavior) const;

private:
    bool UpdateHardwareScreenInfo();
    void BitBlt4BppFast(qint32 iX, qint32 iY, qint32 iWidth, qint32 iHeight, const QImage& srcImg, qint32 isX, qint32 isY);
	void BitBlt8BppFast(qint32 iX, qint32 iY, qint32 iWidth, qint32 iHeight, const QImage& srcImg, qint32 isX, qint32 isY);

    void UpdateToScreen(qint32 iX,  qint32 iY, qint32 iWidth, qint32 iHeight, ScreenProxy::Waveform paintFlag);
    void UpdateToScreenKindleTouch(qint32 iX,  qint32 iY, qint32 iWidth, qint32 iHeight, ScreenProxy::Waveform paintFlag);
    void UpdateToScreenKindleTouch510(qint32 iX,  qint32 iY, qint32 iWidth, qint32 iHeight, ScreenProxy::Waveform paintFlag);
    void UpdateToScreenKindlePaperwhite(qint32 iX,  qint32 iY, qint32 iWidth, qint32 iHeight, ScreenProxy::Waveform paintFlag);

    QRect GetRect()
    {
        return QRect(0, 0, m_nWidth, m_nHeight);
    };

    qint32 m_nWidth;
    qint32 m_nHeight;
    qint32 m_nWidth_virtual;
    qint32 m_nHeight_virtual;

    qint8 *m_addr;
    qint32 m_nAddrLength;
    qint32 m_fb;

};

#endif
