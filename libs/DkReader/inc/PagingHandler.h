#ifndef __PAGINGHANDLER_INCLUDED__
#define __PAGINGHANDLER_INCLUDED__

#include "dkObjBase.h"

namespace DkFormat
{
    class IPagingHandler
    {
    public:
        virtual ~IPagingHandler(){};
        IPagingHandler(){};

        STDMETHOD(GetTotalPage)(PINT piTotal)=0;
        STDMETHOD(SetCurPage)(UINT nPageNo)=0;
        STDMETHOD(GetCurPage)(PUINT piPageNo)=0;
    };
}

#endif // def __PAGINGHANDLER_INCLUDED__

