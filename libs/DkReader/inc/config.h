#ifndef __CONFIG_INCLUDED__
#define __CONFIG_INCLUDED__

#define PLATFORM_WINDOWS            1
#define PLATFORM_DSP_BIOS           2
#define PLATFORM_LINUX              3

#ifdef WIN32
    #define RUN_PLATFORM                PLATFORM_WINDOWS
    #ifdef RELEASE_TO_MARKET
    #include <windows.h>
    #include "swWinError.h"
    #endif
#elif (defined LINUX) || (defined PLATFORM_KINDLE)
    #define RUN_PLATFORM                PLATFORM_LINUX
#else
    #define RUN_PLATFORM                PLATFORM_DSP_BIOS
#endif

#endif      // #ifndef __CONFIG_INCLUDED__

