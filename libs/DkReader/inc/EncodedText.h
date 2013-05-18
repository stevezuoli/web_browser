#ifndef __ENCODEDTEXT_INCLUDED__
#define __ENCODEDTEXT_INCLUDED__

#include <vector>
#include "dkObjBase.h"
#include "dkTextBlock.h"
#include "TextSmartLayout.h"

namespace DkFormat
{
    class IBookmark;

    class IEncodedText
    {
    public:
        IEncodedText() {};
        virtual ~IEncodedText(){};
        
        // 获取包含lPos的TextBlock
        // pBlock： 返回的TextBlock
        // lPos： TextBlock的开始或结束位置，由fLoadForwards决定
        // fForwards： 如果为true，则lPos是TextBlock的开始位置，否则为结束位置
        STDMETHOD(GetTextBlock)(DK_TextBlock *pBlock, LONG lPos, BOOL fLoadForwards)=0;

        // 获取该文本的总长度
        STDMETHOD(GetTextLength)(PLONG plLength)=0;

        // 获取索引表，用于存储索引文件
        STDMETHOD(SetIndexTable)(PINT piIndexTable, INT iIndexCount)=0;

        // 设置索引表，用于从索引文件中加载
        STDMETHOD(GetIndexTable)(PINT *ppiIndexTable, PINT piIndexCount)=0;
        
        // 根据块的起始和块内偏移(转码后)，计算在原文中的实际偏移
        STDMETHOD(GetOriginalOffset)(DK_TextBlock *pBlock, INT iUtf8Offset, PINT piOriginalOffset)=0;

        // 根据块的起始和原文中的实际偏移，计算在块内偏移(转码后）
        STDMETHOD(GetUTF8Offset)(DK_TextBlock *pBlock, INT iOriginalOffset, PINT piUtf8Offset)=0;

        // 设置智能重排需要的规则
        STDMETHOD(SetSmartLayoutMode)(DKSmartLayoutMode eLayoutMode)=0;

        STDMETHOD(GetSmartLayoutMode)(PDKSmartLayoutMode peLayoutMode)=0;

        STDMETHOD(GetChapterList)(std::vector<IBookmark *> &rBookmarkList)=0;

        STDMETHOD(IsPosParaStart)(LONG lPos, PBOOL pfIsParaStart)=0;

        STDMETHOD(GetSearchResult)(IBookmark* searchResultTable[], INT iTableSize, PINT piUsedTableSize, PINT piNeedTableSize, LONG lLoadPos, PCCH pcchUTF8Keyword, UINT uTextLength)=0;
    };
}

#endif // def __ENCODEDTEXT_INCLUDED__

