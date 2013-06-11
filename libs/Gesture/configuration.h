#ifndef _GUICONFIGURATION_H_
#define _GUICONFIGURATION_H_

#include "Device/device.h"

class GUIConfiguration
{
    GUIConfiguration();
public:
    static const GUIConfiguration* GetInstance();

    int GetTouchSlot() const
    {
        return m_touchSlot;
    }

    int GetClickSlot() const
    {
        if (Device::getModel() == Device::KPW)
        {
            return 26;
        }
        else
        {
            return 20;
        }
    }

    int GetClickSlotSquare() const
    {
        return GetClickSlot() * GetClickSlot();
    }

    int GetLongClickSlot() const
    {
        if (Device::getModel() == Device::KPW)
        {
            return 26;
        }
        else
        {
            return 20;
        }
    }

    int GetTapSlot() const
    {
        return GetClickSlot();
    }

    int GetTapSlotSquare() const
    {
        return GetTapSlot() * GetTapSlot();
    }

    int GetDoubleTapSlot() const
    {
        return GetClickSlot();
    }

    int GetDoubleTapSlotSquare() const
    {
        return GetDoubleTapSlot() * GetDoubleTapSlot();
    }

    int GetMinFlingVelocity() const
    {
        return 1;
    }

    int GetMaxFlingVelocity() const
    {
        return 1000;
    }

    int GetLongPressTimeout() const
    {
        return 500;
    }

    int GetHoldingTimeout() const
    {
        return 750;
    }
    int GetTapTimeout() const
    {
        return 180;
    }

    int GetDoubleTapTimeout() const
    {
        return 300;
    }

    int GetMinFlingDistanceX() const
    {
        if (Device::getModel() == Device::KPW)
        {
            return 64;
        }
        else
        {
            return 50;
        }
    }

    int GetMinFlingDistanceY() const
    {
        if (Device::getModel() == Device::KPW)
        {
            return 64;
        }
        else
        {
            return 50;
        }
    }
private:
    int m_touchSlot;
};

GUIConfiguration::GUIConfiguration()
    : m_touchSlot(5)
{
}

const GUIConfiguration* GUIConfiguration::GetInstance()
{
    static GUIConfiguration s_guiConfiguration;
    return &s_guiConfiguration;
}

#endif
