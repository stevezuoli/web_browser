#ifndef __COMMON_WINDOWSMETRICS_H__
#define __COMMON_WINDOWSMETRICS_H__

namespace ui
{
namespace windowsmetrics
{
#define METRICS_LIST(MACRO_NAME)    \
    MACRO_NAME(UIPixelValue2, 2, 3)\
    MACRO_NAME(UIPixelValue8, 8, 10)\
    MACRO_NAME(UIScreenWidth, 600, 758)\
    MACRO_NAME(UIScreenHeight, 800, 1024)\
    MACRO_NAME(UIHorizonMargin, 30, 38)\
    MACRO_NAME(UISoftKeyboardNormalBtnWidth, 50, 64) \
    MACRO_NAME(UISoftKeyboardNormalBtnHeight, 50, 64) \
    MACRO_NAME(UISoftKeyboardSpace, 6, 8) \
    MACRO_NAME(UISoftKeyboardKeySpace, 6, 8) \
    MACRO_NAME(UISoftKeyboardHoriSpace, 10, 13) \
    MACRO_NAME(UISoftKeyboardHeight, 230, 295) \
    MACRO_NAME(UISoftKeyboardPYBottomMargin, 10, 13)\
    MACRO_NAME(UISoftKeyboardIMEPYLabelHeight, 40, 50)\
    MACRO_NAME(UISoftKeyboardIMEPYButtonHeight, 40, 50)\
    MACRO_NAME(UISoftKeyboardTurnPageButtonWidth, 20, 25)\
    MACRO_NAME(WebBrowserNavigationBarIconWidth, 53, 68) \
    MACRO_NAME(WebBrowserNavigationBarIconHeight, 72, 92) \
    MACRO_NAME(WebBrowserNavigationBarMargin, 9, 17) \
    MACRO_NAME(WebBrowserAddressEditHeight, 41, 53) \
    MACRO_NAME(UrlFaviconSize, 16, 21) \
    MACRO_NAME(NavigationBarLinePixel, 2, 3)\
    MACRO_NAME(HistoryItemHeight, 65, 82)\
    MACRO_NAME(HistoryItemSpace, 3, 5)\
    MACRO_NAME(HistoryItemLabelMargin, 20, 24)\
    MACRO_NAME(HistoryItemLabelWidth, 540, 685)\
    MACRO_NAME(HistoryButtonSpace, 16, 21)\
    MACRO_NAME(HistoryButtonHeight, 59, 75)\
    MACRO_NAME(MenuItemPadding, 21, 25)\
    MACRO_NAME(MenuSeparatorPadding, 20, 24)\
    MACRO_NAME(MessageBoxButtonSpace, 10, 12)\
    MACRO_NAME(ListWidgetSingleStepPixel, 30, 38)\
    MACRO_NAME(DKButtonBorderPixel, 1, 1)\
    MACRO_NAME(DKButtonBorderHorPixel, 2, 3)\
    MACRO_NAME(DKButtonBorderBottomPixel, 4, 6)\

enum WindowMetricsIndex
{
#define MAKE_WINDOWS_METRICS_INDEX(x,y,z) x##Index,
    METRICS_LIST(MAKE_WINDOWS_METRICS_INDEX)
#undef MAKE_WINDOWS_METRICS_INDEX
    WindowMetricsCount
};


#define FONTS_LIST(MACRO_NAME)    \
    MACRO_NAME(DKPushButton, 20, 24)    \
    MACRO_NAME(DKLabel, 16, 20)     \
    MACRO_NAME(DKMenu, 24, 30)      \
    MACRO_NAME(HistoryDate, 20, 24) \
    MACRO_NAME(PYWordButton, 24, 30) \

enum WindowFontsIndex
{
#define MAKE_WINDOWS_FONTS_INDEX(x,y,z) x##Index,
    FONTS_LIST(MAKE_WINDOWS_FONTS_INDEX)
#undef MAKE_WINDOWS_FONTS_INDEX
    WindowFontsCount
};

int GetWindowMetrics(int index);
int GetWindowFontSize(int index);
}//windowsmetrics
}//ui
#endif//__COMMON_WINDOWSMETRICS_H__
