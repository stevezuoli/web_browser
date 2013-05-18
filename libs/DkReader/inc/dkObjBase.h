#ifndef __DKOBJBASE_INCLUDED__
#define __DKOBJBASE_INCLUDED__

#include "config.h"
#include "dkBaseType.h"

#ifdef STDMETHODCALLTYPE 
    #undef STDMETHODCALLTYPE 
#endif

#ifdef STDMETHOD 
    #undef STDMETHOD
#endif

#if RUN_PLATFORM == PLATFORM_WINDOWS
#define STDMETHODCALLTYPE __stdcall
#else
#define STDMETHODCALLTYPE
#endif

#define STDMETHOD(method) virtual HRESULT STDMETHODCALLTYPE method

#endif // ndef __DKOBJBASE_INCLUDED__

