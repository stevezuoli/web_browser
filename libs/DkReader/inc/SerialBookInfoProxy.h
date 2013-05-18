#ifndef _SERIALBOOKINFOPROXY_H_
#define _SERIALBOOKINFOPROXY_H_

#include "dkBaseType.h"
#include "dkObjBase.h"

namespace DkFormat
{
    // 索引信息的Proxy类
    class ISerialBookInfoProxy
    {
    public:
        ISerialBookInfoProxy() {};
        virtual ~ISerialBookInfoProxy() {};

        STDMETHOD(SetChapterNum)(UINT uNum)=0;
        STDMETHOD(GetChapterNum)(UINT *puNum)=0;

        STDMETHOD(SetTotalChapterNum)(UINT uNum)=0;
        STDMETHOD(GetTotalChapterNum)(UINT *puNum)=0;

        STDMETHOD(SetUrl)(const CHAR *pUrl)=0;
        STDMETHOD(GetUrl)(CHAR *pUrl, UINT uSize)=0;

        STDMETHOD(SetChapterTitle)(const CHAR *pTitle)=0;
        STDMETHOD(GetChapterTitle)(CHAR *pTitle, UINT uSize)=0;
    };
}

#endif //_SERIALBOOKINFOPROXY_H_

