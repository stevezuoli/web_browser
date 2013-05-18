#ifndef __DKCONTENTMARK_INCLUDED__
#define __DKCONTENTMARK_INCLUDED__

#include "dkBaseType.h"

namespace DkFormat
{
    #define    DK_INVALID_PAGE_NO    -1

    typedef struct
    {
        LPCSTR lpszTitle;                // 章节标题
        INT iPageNo;                    // 章节起始页码
        INT iLevel;                        // 目录层次
    } DK_CONTENT_MARK ;
}

#endif        // #ifndef __DKCONTENTMARK_INCLUDED__

