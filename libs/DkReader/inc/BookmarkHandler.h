#ifndef _BOOKMARKHANDLER_H_
#define _BOOKMARKHANDLER_H_

#include "dkBaseType.h"
#include "Bookmark.h"

namespace DkFormat
{
    class IBookmarkHandler
    {
    public:
        IBookmarkHandler(){};
        virtual ~IBookmarkHandler(){};

        STDMETHOD(Delete)(int iIndex)=0;
        STDMETHOD(Clear)()=0;
        STDMETHOD(GetCount)(int &rNum)=0;
        STDMETHOD(Get)(IBookmark *&rpBookmark, int iIndex)=0;
        STDMETHOD(Add)(IBookmark *pBookmark)=0;
        STDMETHOD(CheckBookmarkforCurPage)(IBookmark *pBookmarkBegin, IBookmark *pBookmarkEnd, INT &iIndex)=0;
        STDMETHOD(Save)()=0;
        STDMETHOD(Load)()=0;
    };
}

#endif // _BOOKMARKHANDLER_H_

