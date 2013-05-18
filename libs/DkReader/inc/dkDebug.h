#ifndef __DKDEBUG_INCLUDED__
#define __DKDEBUG_INCLUDED__

#include <assert.h>
#include "dkBaseType.h"

namespace DkFormat
{
    extern void DkPrintLn(const CHAR *lpszFileName, INT iLineNumber, const CHAR *lpszText);

    extern void DkPerformance(const CHAR *lpszCategory, const CHAR *lpszContext);

    #define PERFORMANCE(x, y) DkPerformance((x), (y))

    #ifdef _DEBUG
        #define DK_TRACE(x) DkPrintLn((__FILE__), (__LINE__), (x))
        #define DK_ASSERT(e) assert(e)
    #else
        #define DK_TRACE(x)
        #define DK_ASSERT(e)
    #endif
}

#endif // __DKDEBUG_INCLUDED__
