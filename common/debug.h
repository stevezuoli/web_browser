#ifndef WIN32
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#define DebugWB(format, args...)\
    printf("%s(%d)::%s"format"\n", __FILE__, __LINE__, __PRETTY_FUNCTION__, ##args);
#else
#define DebugWB printf
#endif
