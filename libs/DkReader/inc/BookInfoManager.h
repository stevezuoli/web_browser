#ifndef __BOOKINFOMANAGER_INCLUDED__
#define __BOOKINFOMANAGER_INCLUDED__

#include "BookInfoEntity.h"
#include "dkBaseType.h"

namespace DkFormat
{
    class CBookInfoManager
    {
    public:
        static HRESULT GetBookInfoEntity(const CHAR *lpszFilename, IBookInfoEntity **ppEntity, BOOL fCreateIfNotExists);
        static HRESULT DeleteBookInfoEntity(const CHAR *lpszFilename);
        static HRESULT ConvertDkpToDkp2(const CHAR *lpszFilename);
    };
}

#endif // __BOOKINFOMANAGER_INCLUDED__

