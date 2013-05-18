#include <stdarg.h>
#include "config.h"
#include "interface.h"
#include "dkBaseType.h"
#include "interface.h"
#include <string>
#include <string.h>

using std::string;

char s_szLogBuff[1024*100];

int DebugPrintf(int iCategoryId, LPCSTR format, ... ) 
{
    int iRetVal =0;

    bool output = false;
    if (iCategoryId == DLC_DIAGNOSTIC || iCategoryId == DLC_GESTURE || iCategoryId == DLC_BENCHMARK)
    {
        output = true;
    }
#ifdef _DEBUG
    output = true;
#endif
    if (output)
    {
        va_list arglist;
        memset(&arglist, 0, sizeof(va_list));

        va_start( arglist, format );
        iRetVal = vsnprintf( s_szLogBuff, sizeof(s_szLogBuff), format, arglist );
        va_end( arglist );

        DoOutputString(s_szLogBuff, iCategoryId);
        if ((unsigned int ) iRetVal >= sizeof(s_szLogBuff) )
        {
            DoOutputString("TRUNCATED!!!", iCategoryId);
        }
    }

    return iRetVal;
}


