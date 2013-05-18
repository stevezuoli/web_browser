#ifndef __DKBASETYPE_INCLUDED__
#define __DKBASETYPE_INCLUDED__

#include "config.h"

#ifndef NULL
    #define NULL    0
#endif

#ifndef TRUE
    #define TRUE        1
#endif

#ifndef FALSE
    #define FALSE       0
#endif

// 矩形的坐标
#ifdef __ASM_FILE__
    // 定义这些常量是因为某些汇编器可能无法使用 struct 的定义, 因此在汇编中访问结构的成员请使用这些常量
    #define RECT_left               0
    #define RECT_top                4
    #define RECT_right              8
    #define RECT_bottom             12

    #define POINT_x                 0
    #define POINT_y                 4

    #define SIZE_cx                 8
    #define SIZE_cy                 12
#else       // #ifdef __ASM_FILE__
    typedef unsigned char   BYTE, BYTE8, *PBYTE8, *LPBYTE8;
    typedef unsigned short  WORD, WORD16, *PWORD16, *LPWORD16;
#ifndef WIN32
    typedef unsigned int    DWORD, DWORD32, *PDWORD32, *LPDWORD32;
#else
    typedef unsigned int    DWORD32, *PDWORD32, *LPDWORD32;
#endif
    typedef signed char         SBYTE, SBYTE8, *PSBYTE8, *LPSBYTE8;
    typedef signed short            SWORD, SWORD16, *PSWORD16, *LPSWORD16;
    typedef signed int              SDWORD, SDWORD32, *PSDWORD32, *LPSDWORD32;
    #if ( RUN_PLATFORM == PLATFORM_WINDOWS )
        typedef unsigned __int64    QWORD64, *PQWORD64, *LPQWORD64;
        typedef __int64             SQWORD64, *PSQWORD64, *LPSQWORD64;
        typedef signed long             HRESULT;
    #elif ( RUN_PLATFORM == PLATFORM_DSP_BIOS )
        typedef unsigned long   QWORD64, *PQWORD64, *LPQWORD64;
        typedef signed long         SQWORD64, *PSQWORD64, *LPSQWORD64;
        typedef SDWORD32        HRESULT;
        typedef unsigned short WCHAR;
        typedef WCHAR *PWCHAR;
        typedef WCHAR *LPWCH, *PWCH;
        typedef const WCHAR *LPCWCH, *PCWCH;
        typedef WCHAR *NWPSTR;
        typedef WCHAR *LPWSTR, *PWSTR;
        typedef const WCHAR *LPCWSTR, *PCWSTR;
    #elif ( RUN_PLATFORM == PLATFORM_LINUX )
        typedef signed long             HRESULT;
        typedef signed char INT8;
        typedef unsigned short WCHAR;
        typedef WCHAR *PWCHAR;
        typedef WCHAR *LPWCH, *PWCH;
        typedef const WCHAR *LPCWCH, *PCWCH;
        typedef WCHAR *NWPSTR;
        typedef WCHAR *LPWSTR, *PWSTR;
        typedef const WCHAR *LPCWSTR, *PCWSTR;
    #endif
    
    typedef signed int INT, *PINT;
    typedef signed int BOOL, *PBOOL;
    typedef unsigned int UINT, *PUINT;

    typedef signed long LONG, *PLONG;
    typedef unsigned long ULONG, *PULONG;
    typedef signed short SHORT, *PSHORT;

    typedef char CHAR;
    //
    // ANSI (Multi-byte Character) types
    //
    typedef CHAR *PCHAR;
    typedef CHAR *LPCH, *PCH;

    typedef const CHAR *LPCCH, *PCCH;
    typedef CHAR *NPSTR;
    typedef CHAR *LPSTR, *PSTR;
    typedef const CHAR *LPCSTR, *PCSTR;

    typedef void *PVOID, *LPVOID;

    #define TEXT(x)         (x)

#ifndef _WIN32
    typedef wchar_t TCHAR;
#endif

    typedef unsigned char UINT8;
    typedef unsigned short UINT16;
    typedef signed short INT16;
    typedef unsigned int UINT32;
    typedef int INT32;
    typedef unsigned long long UINT64;
    typedef signed long long INT64;
    typedef float FLOAT;
    typedef double DOUBLE;


    // left means the first point
    // right means the point after the last point, (out of the area)
    // by this means, width=right-left
    // in the same way, height=bottom-top
    struct DK_RECT{
        signed int    left;
        signed int    top;
        signed int    right;
        signed int    bottom;
        DK_RECT Intersect(const DK_RECT& rhs) const
        {
            int _left = left < rhs.left ? rhs.left : left;
            int _right = right > rhs.right ? rhs.right : right;
            int _top = top < rhs.top ? rhs.top : top;
            int _bottom = bottom > rhs.bottom ? rhs.bottom : bottom;
            DK_RECT rect = {_left, _top, _right, _bottom};
            return rect;
        }

        int GetWidth() const
        {
            return right - left;
        }
        int GetHeight() const
        {
            return bottom - top;
        }
        bool IsEmpty() const
        {
            return left >= right || top >= bottom;
        }
    };

    typedef DK_RECT* PDK_RECT;
    typedef DK_RECT* LPDK_RECT;

    typedef const DK_RECT * PCDK_RECT, *LPCDK_RECT;

    typedef struct {
        signed int  x;
        signed int  y;
    } DK_POINT_OLD, *PDK_POINT_OLD, *LPDK_POINT_OLD;
    
    typedef const DK_POINT_OLD *PCDK_POINT_OLD, *LPCDK_POINT_OLD;

    typedef struct {
        signed int     cx;
        signed int     cy;
    } DK_SIZE, *PDK_SIZE, *LPDK_SIZE;

    typedef const DK_SIZE *PCDK_SIZE, *LPCDK_SIZE;

    #if ( RUN_PLATFORM == PLATFORM_WINDOWS )
        #define strcasecmp stricmp
        #define strncasecmp strnicmp
    #endif

    #define ARRSIZE(a) sizeof(a)/sizeof((a)[0])

#ifndef ABS
    #define ABS(a) (((a)>0) ? (a) : (-(a)))
#endif

#endif      // #ifdef __ASM_FILE__

#endif // __DKBASETYPE_INCLUDED__

