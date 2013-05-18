#ifndef __DKLOGGERCWRAPPER__
#define __DKLOGGERCWRAPPER__
#endif

#include "dkBaseType.h"

#ifdef __cplusplus
extern "C"{
#endif
    void LOG_DebugLog(LPCSTR  module, LPCSTR log);
    void LOG_DebugPrintf(LPCSTR module, LPCSTR log,  ...);
    void LOG_EnableDebugDevice(LPCSTR module, bool enabled);
#ifdef __cplusplus
}
#endif
