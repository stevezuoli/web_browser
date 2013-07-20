#ifndef WIN32
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#define DebugWB(format, args...)\
    printf("%s\t"format"\n", __PRETTY_FUNCTION__, ##args);
#else
#define DebugWB printf
#endif
