#include "common/WindowsMetrics.h"
#include "Device/device.h"

namespace ui
{
namespace windowsmetrics
{
const int g_WindowsMetricsKT[] = {
#define MAKE_WINDOWSMETICS_KT(x,y,z) y,
    METRICS_LIST(MAKE_WINDOWSMETICS_KT)
#undef MAKE_WINDOWSMETICS_KT
};

const int g_WindowsMetricsKP[] = {
#define MAKE_WINDOWSMETICS_KP(x,y,z) z,
    METRICS_LIST(MAKE_WINDOWSMETICS_KP)
#undef MAKE_WINDOWSMETICS_KP
};

const int g_WindowsFontsKT[] = {
#define MAKE_WINDOWSFONTS_KT(x,y,z) y,
    FONTS_LIST(MAKE_WINDOWSFONTS_KT)
#undef MAKE_WINDOWSFONTS_KT
};

const int g_WindowsFontsKP[] = {
#define MAKE_WINDOWSFONTS_KP(x,y,z) z,
    FONTS_LIST(MAKE_WINDOWSFONTS_KP)
#undef MAKE_WINDOWSFONTS_KP
};

int GetWindowMetrics(int index)
{
    if (index < 0 || index >= WindowMetricsCount)
    {
        return -1;
    }

    if (Device::getModel() == Device::KPW)
    {
        return g_WindowsMetricsKP[index];
    }
    return g_WindowsMetricsKT[index];
}


int GetWindowFontSize(int index)
{
    if (index < 0 || index >= WindowFontsCount)
    {
        return -1;
    }

    if (Device::getModel() == Device::KPW)
    {
        return g_WindowsFontsKP[index];
    }
    return g_WindowsFontsKT[index];
}
}
}
