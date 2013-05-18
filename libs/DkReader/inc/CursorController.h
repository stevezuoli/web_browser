#ifndef __CURSORCONTROLER_INCLUDED__
#define __CURSORCONTROLER_INCLUDED__

#include "dkBaseType.h"
#include "../Common/dkLayout.h"

namespace DkFormat
{
    class ICursorController{
    public:
        ICursorController(){};
        virtual ~ICursorController(){};

        STDMETHOD(SetCursorStatus)(DKLayoutCursorStatus cursorStatus)=0;
        STDMETHOD(GetCursorStatus)(PDKLayoutCursorStatus pCursorStatus)=0;
        STDMETHOD(SetCursorIndex)(INT iIndexInCurPage)=0;
        STDMETHOD(GetCursorIndex)(PINT piIndexInCurPage)=0;
        STDMETHOD(SetCursorDraggingStartIndex)(INT iDraggingStartIndex)=0;
        STDMETHOD(GetCursorDraggingStartIndex)(PINT piDraggingStartIndex)=0;

        // 得到当前光标的矩形区域
        STDMETHOD(GetCurCursorRect)(PDK_RECT pCursorRect)=0;

        // 移动光标，FormatRender 中，光标指向的文本下表相应改变
        STDMETHOD(CursorMoving)(DKLayoutTextUnit textUnit, INT iStep, BOOL fWordEdge, PDKLayoutCursorPosition pCursorPosition, PDK_RECT pCursorRect = NULL)=0;

        // 得到光标拖拽时经过的文本区域，用于绘制高亮显示背景色。文本区域存放在矩形数组 ppRectTable 中。
        STDMETHOD(CursorDragging)(DKLayoutTextUnit textUnit, INT iStep, DK_RECT **ppRectTable, PUINT puLength, PDKLayoutCursorPosition pCursorPosition)=0;

        // 得到拖拽区域的文字内容。数据存储在调用者给定的内存空间里，uGivenLength 标识了可供存储的空间大小。
        STDMETHOD(GetDraggingContent)(PBYTE8 pbContent, UINT uGivenLength, PUINT puContentLength=NULL)=0;

        // 得到光标所在位置右侧的一个单词（英文）。如果光标位置处为英文，返回值为S_OK，若为非英文，返回值为S_FALSE
        STDMETHOD(GetWordAtCursor)(PBYTE8 pbContent, INT iGivenLength, PINT piContentLength=NULL)=0;
    };
}

#endif

