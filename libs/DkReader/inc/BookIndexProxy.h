#ifndef __BOOKINDEXPROXY_INCLUDED__
#define __BOOKINDEXPROXY_INCLUDED__

#include "dkBaseType.h"
#include "dkObjBase.h"

namespace DkFormat
{
    // 索引信息的Proxy类
    class IBookIndexProxy
    {
    public:
        IBookIndexProxy() {};
        virtual ~IBookIndexProxy() {};

        STDMETHOD(GetIndexList)(INT **ppiIndexList, PUINT pnCount)=0;
        STDMETHOD(SetIndexList)(INT *piIndexList, UINT nCount)=0;
    };
}

#endif // __BOOKINDEXPROXY_INCLUDED__

