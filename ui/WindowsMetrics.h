#ifndef __COMMON_WINDOWSMETRICS_H__
#define __COMMON_WINDOWSMETRICS_H__

namespace ui
{
namespace windowsmetrics
{
#define METRICS_LIST(MACRO_NAME)    \
    MACRO_NAME(UIScreenWidth, 600, 758)\
    MACRO_NAME(UIScreenHeight, 800, 1024)\
    MACRO_NAME(UISoftKeyboardNormalBtnWidth, 50, 64) \
    MACRO_NAME(UISoftKeyboardNormalBtnHeight, 50, 64) \
    MACRO_NAME(UISoftKeyboardHoriBorder, 10, 12) \
    MACRO_NAME(UISoftKeyboardBigHoriBorder, 24, 31) \
    MACRO_NAME(UISoftKeyboardSpace, 6, 8) \
    MACRO_NAME(UISoftKeyboardHeight, 230, 295)

enum WindowMetricsIndex
{
#define MAKE_WINDOWS_METRICS_INDEX(x,y,z) x##Index,
    METRICS_LIST(MAKE_WINDOWS_METRICS_INDEX)
#undef MAKE_WINDOWS_METRICS_INDEX
    WindowMetricsCount
};

int GetWindowMetrics(int index);
}//windowsmetrics
}//ui
#endif//__COMMON_WINDOWSMETRICS_H__
