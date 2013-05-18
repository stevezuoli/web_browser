#ifndef _BOOKMARK_H_
#define _BOOKMARK_H_

#include <time.h>
#include "dkObjBase.h"
#include "dkBaseType.h"
#include "BookLocator.h"

namespace DkFormat
{
    typedef enum 
    {
        DK_BOOK_MARK,
        DK_BOOK_COMMENT,
        DK_BOOK_SUMMARY
    } EBookmarkType;

    class IBookmark
    {
    public:
        IBookmark(){};
        virtual ~IBookmark(){};

        STDMETHOD(GetLocatorCount)(int *piCount) = 0;
        STDMETHOD(GetCommentLength)(int *piLength) = 0;
        STDMETHOD(GetComment)(char **ppCmt) = 0;
        STDMETHOD(SetComment)(const char *pCmt, int iLen) = 0;
        STDMETHOD(SetLocator)(IBookLocator *pLocator, int iIndex) = 0;
        STDMETHOD(GetLocator)(IBookLocator **pLocator, int iIndex) = 0;
        STDMETHOD(SetTime)(time_t &rTime) = 0;
        STDMETHOD(GetTime)(time_t &rTime) = 0;
        STDMETHOD(GetLocatorType)(BookLocatorType &rType) = 0;
        STDMETHOD(Clone)(IBookmark *&prBookmark) = 0;
        STDMETHOD(GetType)(EBookmarkType &reType) = 0;
        STDMETHOD(SetType)(EBookmarkType eType) = 0;
        
        STDMETHOD(ReadData)(const BYTE8 *pbData, UINT nLength, int *piReadData) = 0;
        STDMETHOD(WriteData)(BYTE8 *pbData, UINT nLength, int *piWriteBytes) = 0;
        STDMETHOD(GetDataSize)(int *piSize) = 0;
    };
}

#endif // _BOOKMARK_H_

