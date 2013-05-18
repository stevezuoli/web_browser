#ifndef __INTERFACE_INCLUDED__
#define __INTERFACE_INCLUDED__

#include "dkBaseType.h"
#include "DKLoggerCWrapper.h"

#ifdef __cplusplus
#ifndef __ASM_FILE__
extern "C" {
#endif      // #ifndef __ASM_FILE__
#endif      // #ifdef __cplusplus

// 采集一场图象时的附加信息
#ifdef __ASM_FILE__
    // 定义这些常量是因为某些汇编器可能无法使用 struct 的定义, 因此在汇编中访问结构的成员请使用这些常量
    #define FIELD_INFO_iTime                0
    #define FIELD_INFO_dwID                 4
#else
    typedef struct {
        int iTime;      // 时间. 用 1970/01/01 00:00:00 以来的秒数表示
        DWORD32 dwID;       // 标记(它不一定连续). 用时间的标记可以唯一地确定一场图象.
    } HV_FIELD_INFO;
#endif      // #ifdef __ASM_FILE__

#define LOG_CAT_LIST(MACRO_NAME)        \
    MACRO_NAME(DBG_CONSOLE)             \
    MACRO_NAME(COMM)                    \
    MACRO_NAME(ERROR)                   \
    MACRO_NAME(TIME)                    \
    MACRO_NAME(INPUT)                   \
    MACRO_NAME(FRAMEBUF)                \
    MACRO_NAME(EINK_PAINT)              \
    MACRO_NAME(GUI)                     \
    MACRO_NAME(GUI_VERBOSE)             \
    MACRO_NAME(WIFI)                    \
    MACRO_NAME(DOWNLOAD)                \
    MACRO_NAME(VERBOSE)                 \
    MACRO_NAME(MESSAGE)                 \
    MACRO_NAME(FOCUS)                   \
    MACRO_NAME(SETTINGS)                \
    MACRO_NAME(FORMAT_TXT)              \
    MACRO_NAME(THREAD)                  \
    MACRO_NAME(OTHER)                   \
    MACRO_NAME(TRACE)                   \
    MACRO_NAME(WANGYJ)                  \
    MACRO_NAME(ZHANGXB)                 \
    MACRO_NAME(CX)                      \
    MACRO_NAME(LIUJT)                   \
    MACRO_NAME(LIUHJ)                   \
    MACRO_NAME(XU_KAI)                  \
    MACRO_NAME(ZHANGJF)                 \
    MACRO_NAME(STRING)                  \
    MACRO_NAME(CHENM)                   \
    MACRO_NAME(PENGF)                   \
    MACRO_NAME(ZHAIGH)                  \
    MACRO_NAME(JUGH)                    \
    MACRO_NAME(FOR_KEYBOARD_DEBUG)      \
    MACRO_NAME(LIWEI)                   \
    MACRO_NAME(LUOXP)                   \
    MACRO_NAME(ZHY)                   \
    MACRO_NAME(UIHOMEPAGE)              \
    MACRO_NAME(UICOMPLEXDLG)            \
    MACRO_NAME(UITEXTBOX)               \
    MACRO_NAME(UIWINDOW)                \
    MACRO_NAME(UICONTAINER)             \
    MACRO_NAME(UIDIALOG)                \
    MACRO_NAME(UITITLEBAR)              \
    MACRO_NAME(UIBOTTOMBAR)             \
    MACRO_NAME(UIBOOKMENU)              \
    MACRO_NAME(UIBOOKLISTBOX)           \
    MACRO_NAME(UIBOOKLISTITEM)          \
    MACRO_NAME(UIDIRECTORYPAGE)         \
    MACRO_NAME(UIDIRECTORYLISTBOX)      \
    MACRO_NAME(UIDIRECTORYLISTITEM)     \
    MACRO_NAME(UIMESSAGEBOX)            \
    MACRO_NAME(UIWIFIDIALOG)            \
    MACRO_NAME(UISYSTEMSETTING)         \
    MACRO_NAME(UIBUTTONDLG)             \
    MACRO_NAME(UIDATESETTINGDLG)        \
    MACRO_NAME(UINUMINPUT)              \
    MACRO_NAME(UISINGLETEXTINPUTDLG)    \
    MACRO_NAME(UITOUCHCOMPLEXBUTTON)    \
    MACRO_NAME(UIPAGE)                  \
    MACRO_NAME(BOOKINFODLG)             \
    MACRO_NAME(BOOKOPENMANAGER)         \
    MACRO_NAME(DKREADERPAGE)            \
    MACRO_NAME(UIIMAGEREADERCONTAINER)  \
    MACRO_NAME(CDKFILEMANAGER)          \
    MACRO_NAME(SELECTEDCONTROLER)       \
    MACRO_NAME(UIBOOKREADERSELECTED)    \
    MACRO_NAME(UICOMMENTORSUMMARY)      \
    MACRO_NAME(UIBOOKREADERCONTAINER)   \
    MACRO_NAME(UIBOOKREADERPROGRESSBAR) \
    MACRO_NAME(UICOMPOUNDLISTBOX)       \
    MACRO_NAME(BOOKSTORE)               \
    MACRO_NAME(UIFOOTNOTEDIALOG)        \
    MACRO_NAME(UITEXT)                  \
    MACRO_NAME(TOUCHSTACK_LONGTAP)      \
    MACRO_NAME(DIAGNOSTIC)              \
    MACRO_NAME(UITEXTMULTILINE)         \
    MACRO_NAME(TIME_TEST)         \
    MACRO_NAME(GESTURE)         \
    MACRO_NAME(FRONTLIGHT)              \
    MACRO_NAME(BENCHMARK)         \
    MACRO_NAME(ACCOUNT)         \
    MACRO_NAME(UISOFTKEYBOARDIME)         \


#define DEFINE_LOG_CATEGORY_ENUM(a)     \
    DLC_##a,                \

enum DkLogCategory
{
    LOG_CAT_LIST(DEFINE_LOG_CATEGORY_ENUM)
    DLC_Count
};

extern int g_rgfDebugDeviceEnabled[ DLC_Count ];
extern LPCSTR g_rgkszDebugDeviceName[ DLC_Count ];
extern char g_rgkszDebugLogPath[ DLC_Count ][ 260 ];
extern BOOL g_fSaveLogFile;

__inline LPCSTR GetDebugDeviceName( int iDeviceID )
{
    return g_rgkszDebugDeviceName[iDeviceID];
}

__inline void EnableDebugDevice( int iDeviceID, int fEnable ) 
{
    g_rgfDebugDeviceEnabled[ iDeviceID ] = fEnable;

    LOG_EnableDebugDevice(g_rgkszDebugDeviceName[iDeviceID], fEnable);
}

__inline int IsDebugDeviceEnabled( int iDeviceID ) 
{
    return g_rgfDebugDeviceEnabled[ iDeviceID ];
}

__inline LPCSTR GetDebugLogPath(int iDeviceID)
{
    return g_rgkszDebugLogPath[iDeviceID];
}

__inline void SetSaveLogFile(BOOL fSave)
{
    g_fSaveLogFile=fSave;
}

extern int UartWriteData( BYTE8 *pDataSrc, int iDataLenb );
extern int UartReadData( BYTE8 *pDataDest, int iReadLen );
extern int UartWriteString( LPSTR pszString );

// 下面的常量定义了 OutputString 中, 参数 iDeviceID 可能的取值.
#define NULL_DEVICE                 255 //a

// 宏定义: int OutputString( char *pString, int iDeviceID )
// 描述: 在不同的设备上输出"结果字符串"
// 参数: pString    指向需要输出的字符串. 如果该参数为 NULL, 函数返回 INVALD_POINTER
//       iDeviceID  给出需要在什么设备上输出"结果字符串". 其可能的取值由上面的常量定义. 如果其取值不是上面的常量中的
//                  任何一个, 则本函数返回 INVALID_DEVICE.
//                  iDeviceID == NULL_DEVICE 本宏是一个空宏, 没有调用任何函数, 不导致任何运行开销. 这时 pImageInfo 和
//                  pImage 可以取任何值, 不会导致错误.
//                  iDeviceID == RESULT_STR_FILE 时 pString 将被写入结果文件中. 结果文件的存放位置由实现决定.
//                  iDeviceID == DISPLAY_STR_COUNT 时 pString 将显示在界面的"车辆计数"文本域中.
//                  iDeviceID == DISPLAY_STR_RESULT 时 pString 将显示在界面的"识别结果"文本域中.
//                  iDeviceID == DEBUG_STR_DISPLAY1 时 pString 将显示在界面的"调试信息"文本域中.
//                  iDeviceID == DEBUG_STR_DISPLAY2 时 pString 将显示在界面的"调试信息2"文本域中. 目前没有实现"调试信
//                  息2"文本域
//                  iDeviceID == DEBUG_STR_COMM_FILE 时 pString 将被写入"通用调试信息"文件中. "通用调试信息"文件的存放
//                  位置由实现决定.
//                  iDeviceID == DEBUG_STR_DETECT_FILE 时 pString 将被写入"检测调试信息"文件中. "检测调试信息"文件的存放
//                  位置由实现决定.
//                  iDeviceID == DEBUG_STR_SEGMENT_FILE 时 pString 将被写入"分割调试信息"文件中. "分割调试信息"文件的存放
//                  位置由实现决定.
//                  iDeviceID == DEBUG_STR_RECOGNIZ_FILE 时 pString 将被写入"识别调试信息"文件中. "识别调试信息"文件的存放
//                  位置由实现决定.
//                  iDeviceID == DEBUG_STR_OTHER_FILE 时 pString 将被写入"其他调试信息"文件中. "其他调试信息"文件的存放
//                  位置由实现决定.
//                  之所以将调试信息做如此详细的划分, 是因为不同阶段的任务可能在不同的线程中运行, 详细的划分可以避免调试
//                  信息文件的内容因线程推进顺序问题变得杂乱无章
// 返回: 成功时返回 0.
//       如果 pString 为 NULL, 函数返回 INVALD_POINTER
//       如果 iDeviceID 的取值非法, 则返回 INVALID_DEVICE.
//       如果无法在指定设备上输出, 则返回 DEVICE_FAIL.
// 注意: 由于目前的实现是在预处理阶段判断 iDeviceID 是否为空设备, 因此调用本宏时 iDeviceID 必须是一个常量, 否则将导致
//       编译错误.
#ifdef __ASM_FILE__
    .global _DoOutputString
#else
    int DoOutputString( LPCSTR pString, int iDeviceID );

    BOOL OpenLogFile(int iDeviceID, LPCSTR lpszFileName, LPCSTR lpszMode);
    BOOL CloseLogFile(int iDeviceID);
    BOOL IsLogFileOpened(int iDeviceID);

#ifdef _DEBUG
    #define DebugLog( iCategoryID, pString )        \
        if ( iCategoryID != NULL_DEVICE ) {            \
            DoOutputString( pString, iCategoryID );    \
        }
#else
    #define DebugLog( iCategoryID, pString )
#endif

    // TODO: This could make garbage in RTM
    int DebugPrintf(int iCategoryId, LPCSTR format, ... );

    #define SafeDeleteArrayEx(_pPointer)    \
        if ( NULL != _pPointer) {        \
            delete[] _pPointer;            \
            _pPointer = NULL;            \
        }

    #define SafeDeletePointerEx(_pPointer)    \
        if ( NULL != _pPointer) {            \
            delete _pPointer;                \
            _pPointer = NULL;                \
        }
        
    #define SafeFreePointerEx(_pPointer)        \
        if ( NULL != _pPointer) {            \
            free(_pPointer);                \
            _pPointer = NULL;                \
        }

#endif      // #ifdef __ASM_FILE__

// 宏定义: int GetSystemTick( void )
// 描述: 取得系统 Tick
// 参数: 无
// 返回: 成功时返回系统 Tick. 目前无其他返回.
// 注意: 系统 Tick 有系统实现有关, 其绝对时间长度不确定. 目前在 DSP_BIOS 和 WINDOWS 中其取值都是 1 ms.
#ifdef __ASM_FILE__
    .global _GetSystemTick
#else
    DWORD32 GetSystemTick( void );
#endif      // #ifdef __ASM_FILE__

#ifdef __ASM_FILE__
    .global _BreakPoint
#else
    void BreakPoint();
#endif      // #ifdef __ASM_FILE__

#ifdef __ASM_FILE__
    .global _BreakPoint
#else
    int Random();
#endif      // #ifdef __ASM_FILE__

void SQM_IncCounter(int iActionId);
void SQM_LogEvent(int iActionId, const char * kszMessage);
void SQM_Flush();

#define RTN_HR_IF_FAILED(func)                          \
    hr=func;                                            \
    if (FAILED(hr))                                     \
    {                                                   \
        DebugPrintf(DLC_ERROR, "FAILED (%s:%d hr=0x%08X): " \
            #func, __FILE__, __LINE__, hr);                                 \
        return hr;                                      \
    }                                                   \

#ifdef __cplusplus
#ifndef __ASM_FILE__
}   // extern "C"
#endif      // #ifndef __ASM_FILE__
#endif      // #ifdef __cplusplus


#endif      // #ifndef __INTERFACE_INCLUDED__

