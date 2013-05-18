#ifndef __DKTEXTBLOCK_INCLUDED__
#define __DKTEXTBLOCK_INCLUDED__

#include <stdlib.h>
#include "dkBaseType.h"

namespace DkFormat
{
    typedef struct DK_TextBlock{
        
        INT iOriginalStart;    // 此文本在源IEncodedText中的起始偏移
        INT iOriginalEnd;        // 此文本在源IEncodedText中的结束偏移, iOriginalStart和iOriginalEnd是左闭右开的一个区间
        PBYTE8 pUTF8Text;    // 此文本经过转码之后的UTF-8文本
        INT iTextCapacity;    // pUTF8Text分配的内存大小
        INT iTextLength;        // UTF-8文本长度
        INT iStartIndex;        // UTF-8文本中的有效文本段，由iStartIndex和iEndIndex标识（iStartIndex和iEndIndex是左闭右开的一个区间）
        INT iEndIndex;        // UTF-8有效文本段的结束下标，此下标为有效文本最后一个字符下标加一（即有效文本段不包括此下标位置处的字符）
        INT *pMappingTable;    // 转码前后字符偏移的映射表: 偶数下标(从0开始)存转码后的偏移，奇数下标存转码前的偏移
        INT iTableCapacity;     // 映射表的内存大小
        INT iTableLength;    // 映射表长度
        BOOL fIsLoaded;        // 为TRUE时表示当前块是否被加载，若加载，则其他数据段为有效数据

        DK_TextBlock()
            : iOriginalStart(0)
            , iOriginalEnd(0)
            , pUTF8Text(NULL)
            , iTextCapacity(0)
            , iTextLength(0)
            , iStartIndex(0)
            , iEndIndex(0)
            , pMappingTable(NULL)
            , iTableCapacity(0)
            , iTableLength(0)
            , fIsLoaded(FALSE)
        {            
        }

        ~DK_TextBlock()
        {
            if (pUTF8Text != NULL)
            {
                free(pUTF8Text);
                pUTF8Text = NULL;
            }
            
            if (pMappingTable != NULL)
            {
                free(pMappingTable);
                pMappingTable = NULL;
            }
        }    
    } DK_TextBlock;
}

#endif         // #ifndef __DKTEXTBLOCK_INCLUDED__

