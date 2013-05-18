#ifndef __BOOKTAGPROXY_INCLUDED__
#define __BOOKTAGPROXY_INCLUDED__

#include "dkBaseType.h"
#include "dkObjBase.h"

namespace DkFormat
{
    // 标签信息的Proxy类
    class IBookTagProxy
    {
    public:
        IBookTagProxy() {};
        virtual ~IBookTagProxy() {};

        STDMETHOD(GetTagList)(PCHAR **ppList, PUINT pnCount)=0;
        STDMETHOD(SetTagList)(PCHAR *pList, UINT nCount)=0;
        STDMETHOD(ReleaseTagList)(CHAR **ppList, UINT nCount)=0;
    };
}

#endif // __BOOKTAGPROXY_INCLUDED__

