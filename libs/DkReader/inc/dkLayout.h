#ifndef __DKLAYOUT_INCLUDED__
#define __DKLAYOUT_INCLUDED__

#include "dkAttribute.h"

namespace DkFormat
{
    #define DK_LAYOUT_SCALE          1024       // Same with PANGO_SCALE

    typedef enum 
    {
        DK_LAYOUT_WRAP_WORD,        // wrap lines at word boundaries
        DK_LAYOUT_WRAP_CHAR,        // wrap lines at character boundaries.
        DK_LAYOUT_WRAP_WORD_CHAR,    // wrap lines at word boundaries, but fall back to character boundaries if there is not enough space for a full word.
    } DKLayoutWrapMode;

    typedef enum {
      DK_LAYOUT_ELLIPSIZE_NONE,        // No ellipsization
      DK_LAYOUT_ELLIPSIZE_START,    // Omit characters at the start of the text
      DK_LAYOUT_ELLIPSIZE_MIDDLE,    // Omit characters in the middle of the text
      DK_LAYOUT_ELLIPSIZE_END        // Omit characters at the end of the text
    } DKLayoutEllipsizeMode;

    typedef enum {
      DK_LAYOUT_ALIGN_LEFT,            // Put all available space on the right
      DK_LAYOUT_ALIGN_CENTER,        // Center the line within the available space
      DK_LAYOUT_ALIGN_RIGHT            // Put all available space on the left
    } DKLayoutAlignment;

    typedef enum{
        DK_LAYOUT_LINE_NORMAL,                                //行模式正常, 与图文混排无关
        DK_LAYOUT_LINE_SHAPE_START_VIRTUAL,        //文本环绕式图文混排侧面虚行起始行
        DK_LAYOUT_LINE_SHAPE_FOLLOW_VIRTUAL,    //文本环绕式图文混排侧面虚行后续行
        DK_LAYOUT_LINE_SHAPE_START,                        //文本环绕式图文混排侧面起始行
        DK_LAYOUT_LINE_SHAPE_FOLLOW,                    //文本环绕式图文混排侧面后续行
        DK_LAYOUT_LINE_SHAPE_IN_LINE                    //嵌入式图文混排行
    } DKLayoutLineMode;

    typedef enum{
        DK_LAYOUT_TEXT_UNIT_GRAPHEME,                //文本以字形为单位
        DK_LAYOUT_TEXT_UNIT_WORD,                        //文本以单词为单位
        DK_LAYOUT_TEXT_UNIT_SENTENCE,                //文本以句子为单位
        DK_LAYOUT_TEXT_UNIT_LINE,                          //文本以一行为单位
        DK_LAYOUT_TEXT_UNIT_PARAGRAPH             //文本以段落为单位
    } DKLayoutTextUnit;

    typedef enum{
        CURSOR_UNVISIBLE,                                    //光标不可见
        CURSOR_VISIBLE_IN_SCREEN,                            //光标在当前屏，移动状态，可见
        CURSOR_VISIBLE_OUT_SCREEN,                            //光标不在当前屏，移动状态，可见
        CURSOR_DRAGGING_IN_SCREEN,                            //光标在当前屏，拖拽状态，可见
        CURSOR_DRAGGING_OUT_SCREEN                            //光标不在当前屏，拖拽状态，可见
    }DKLayoutCursorStatus, *PDKLayoutCursorStatus;

    typedef enum{
        CURSOR_POSITION_EMBEDED_TEXT,                    //光标位置在文本中间
        CURSOR_POSITION_OVER_HEAD,                        //光标位置已越过当前页页首
        CURSOR_POSITION_OVER_TAIL                            //光标位置已越过当前页页尾
    }DKLayoutCursorPosition, *PDKLayoutCursorPosition;

    typedef struct {
        INT                    iStartIndex;                // 该行对应当前排版的Text中起始偏移
        INT                    iLineHeight;                // 该行在排版中的高度
        INT                    iLineWidth;                // 该行在排版中的宽度
        INT                    iLineXOffset;                // 该行在排版中x轴偏移
        INT                    iLineYOffset;                // 该行在排版中y轴偏移
        INT                 iSpacing;
        DKLayoutLineMode    lineMode;                // 该行在排版中的行模式
    } DKLayoutLineInfo;

    typedef struct{
        INT iX;
        INT iY;
        INT iWidth;
        INT iHeight;
    } DKLayoutRectangle;

    typedef struct DKLayoutColorBlockInfo{
        DK_RECT                            rect;
        DKColor                                color;
        DKLayoutColorBlockInfo*    next;
    } DKLayoutColorBlock, *PDKLayoutColorBlock;

    typedef struct DK_LayoutShapeInfo{
        int                        iStartOffset;            // 该图形在文本中的开始偏移
        DKLayoutRectangle        shapeRect;                // 该图形在排版中的位置矩形
        PDKShape                pShape;

        DK_LayoutShapeInfo()
                :iStartOffset(0), pShape(NULL)
        {
            memset(&shapeRect, 0, sizeof(DKLayoutRectangle));
        }
    } DKLayoutShapeInfo;
}

#endif        // #ifndef __DKLAYOUT_INCLUDED__

