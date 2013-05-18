#ifndef _BOOKMARKPROXY_H_
#define _BOOKMARKPROXY_H_

#include <vector>
#include "dkBaseType.h"
#include "dkObjBase.h"
#include "Bookmark.h"

namespace DkFormat
{
    // 书签/书摘/批注的Proxy类
    class IBookmarkProxy
    {
    public:
        IBookmarkProxy() {};
        virtual ~IBookmarkProxy() {};

        STDMETHOD(GetBookmarkCount)(PUINT pnCount) = 0;
        STDMETHOD(GetBookmarkList)(std::vector<IBookmark *> &rList) = 0;
        STDMETHOD(SetBookmarkList)(std::vector<IBookmark *> &rList) = 0;
    };
}

#endif // _BOOKMARKPROXY_H_

