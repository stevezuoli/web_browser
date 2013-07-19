#include <QtGlobal>
#ifndef WIN32
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#define DebugWB(format, args...)\
    qDebug("%s\t"format"\n", __PRETTY_FUNCTION__, ##args);
#else
#define DebugWB qDebug
#endif
