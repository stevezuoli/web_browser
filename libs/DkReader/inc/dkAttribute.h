#ifndef __DKATTRIBUTE_INCLUDED__
#define __DKATTRIBUTE_INCLUDED__

#include "dkObjBase.h"
#include "dkColor.h"
#include "dkWinError.h"
#include "DkSPtr.h"
#include "dkBuffer.h"

namespace DkFormat
{

#define MAX_IMG_SRC_LEN 128
#define MAX_IMG_REF_LEN 512

typedef enum DK_Attribute_Type
{
    DK_NONE,
    DK_BGCOLOR,
    DK_FGCOLOR,
    DK_LETTER_SPACING,
    DK_TEXT_ALIGN,
    DK_UNDERLINE,
    DK_COMMENT,
    DK_STRIKE_THROUGH,
    DK_TEXT_INDENT,
    DK_FONT_FAMILY,
    DK_FONT_SIZE,
    DK_FONT_STRETCH,
    DK_FONT_STYLE_ATTR,
    DK_FONT_VARIANT,
    DK_FONT_WEIGHT,
    DK_LIST_STYLE_IMAGE,
    DK_LIST_STYLE_POSITION,
    DK_LIST_STYLE_TYPE,
    DK_SHAPE,
    DK_CURSOR,
    DK_DISPLAY,
    DK_FLOAT_ATTR
} DKAttributeType, *PDKAttributeType;

typedef enum DK_Align_Type
{
    DK_ALIGN_LEFT,
    DK_ALIGN_CENTER,
    DK_ALIGN_RIGHT,
    DK_ALIGN_JUSTIFY
} DKAlignType, *PDKAlignType;

typedef enum DK_Underline
{
    DK_UNDERLINE_NONE,
    DK_UNDERLINE_SINGLE,
    DK_UNDERLINE_DOUBLE,
    DK_UNDERLINE_LOW,
    DK_UNDERLINE_UP,
    DK_UNDERLINE_ERROR
} DKUnderline, *PDKUnderline;

typedef enum DK_Cursor_Type
{
    DK_CURSOR_NONE,
    DK_CURSOR_TEXT,
    DK_CURSOR_POINTER,
    DK_CURSOR_ARROW
} DKCursorType, *PDKCursorType;

typedef enum DK_Display_Type
{
    DK_DISPLAY_NONE,
    DK_DISPLAY_INLINE,
    DK_DISPLAY_BLOCK,
    DK_DISPLAY_LIST
} DKDisplayType, *PDKDisplayType;

typedef enum DK_Float_Type
{
    DK_FLOAT_NONE,
    DK_FLOAT_LEFT,
    DK_FLOAT_RIGHT
} DKFloatType, *PDKFloatType;

typedef enum DK_Font_Style_Type
{
    DK_FONT_STYLE_NORMAL,
    DK_FONT_STYLE_OBLIQUE,
    DK_FONT_STYLE_ITALIC
} DKFontStyleType, *PDKFontStyleType;

typedef enum DK_Font_Variant_Type
{
    DK_FONT_VARIANT_NOMAL,
    DK_FONT_VARIANT_SMALLCAPS
} DKFontVariantType, *PDKFontVariantType;

typedef enum DK_Font_Weight_Type
{
    DK_WEIGHT_THIN = 100,
    DK_WEIGHT_ULTRALIGHT = 200,
    DK_WEIGHT_LIGHT = 300,
    DK_WEIGHT_BOOK = 380,
    DK_WEIGHT_NORMAL = 400,
    DK_WEIGHT_MEDIUM = 500,
    DK_WEIGHT_SEMIBOLD = 600,
    DK_WEIGHT_BOLD = 700,
    DK_WEIGHT_ULTRABOLD = 800,
    DK_WEIGHT_HEAVY = 900,
    DK_WEIGHT_ULTRAHEAVY = 1000
} DKFontWeightType, *PDKFontWeightType;

typedef enum DK_Font_Stretch_Type
{
    DK_STRETCH_ULTRA_CONDENSED,
    DK_STRETCH_EXTRA_CONDENSED,
    DK_STRETCH_CONDENSED,
    DK_STRETCH_SEMI_CONDENSED,
    DK_STRETCH_NORMAL,
    DK_STRETCH_SEMI_EXPANDED,
    DK_STRETCH_EXPANDED,
    DK_STRETCH_EXTRA_EXPANDED,
    DK_STRETCH_ULTRA_EXPANDED
} DKFontStretchType, *PDKFontStretchType;

typedef struct DK_Shape
{
    INT width;
    INT height;
    DKFloatType floatType;
    CHAR pSrc[MAX_IMG_SRC_LEN];
    CHAR pRef[MAX_IMG_REF_LEN];

    DK_Shape()
    {
        width = -1;
        height = -1;
        floatType = DK_FLOAT_NONE;
        pSrc[0] = 0;
        pRef[0] = 0;
    }

} DKShape, *PDKShape, **PPDKShape;

class IAttribute
{
public:
    virtual ~IAttribute(){};
    IAttribute(){};

    STDMETHOD(SetBgcolor)(SPtr<DKColor> spBgcolor) = 0;
    STDMETHOD(GetBgcolor)(SPtr<DKColor>& spBgColor) = 0;

    STDMETHOD(SetFgcolor)(SPtr<DKColor> spFgcolor) = 0;
    STDMETHOD(GetFgcolor)(SPtr<DKColor>& spFgcolor) = 0;

    STDMETHOD(SetLetterSpacing)(UINT uLetterSpacing) = 0;
    STDMETHOD(GetLetterSpacing)(PUINT puLetterSpacing) = 0;

    STDMETHOD(SetTextAlign)(DKAlignType eTextAlign) = 0;
    STDMETHOD(GetTextAlign)(PDKAlignType peTextAlign) = 0;

    STDMETHOD(SetUnderline)(DKUnderline eUnderline) = 0;
    STDMETHOD(GetUnderline)(PDKUnderline peUnderline) = 0;

    STDMETHOD(SetStrikeThrough)(BOOL fStrikeThrough) = 0;
    STDMETHOD(GetStrikeThrough)(BOOL *pfStrikeThrough) = 0;

    STDMETHOD(SetTextIndent)(UINT uIndent) = 0;
    STDMETHOD(GetTextIndent)(PUINT puIndent) = 0;

    STDMETHOD(SetFontFamily)(const CHAR *pFontFamily) = 0;
    STDMETHOD(GetFontFamily)(PCHAR *ppFontFamily) = 0;

    STDMETHOD(SetFontSize)(UINT uFontSize) = 0;
    STDMETHOD(GetFontSize)(PUINT puFontSize) = 0;

    STDMETHOD(SetFontStretch)(DKFontStretchType eFontStretch) = 0;
    STDMETHOD(GetFontStretch)(PDKFontStretchType peFonttretch) = 0;

    STDMETHOD(SetFontStyle)(DKFontStyleType eFontStyle) = 0;
    STDMETHOD(GetFontStyle)(PDKFontStyleType peFontStyle) = 0;

    STDMETHOD(SetFontVariant)(DKFontVariantType eFontVariant) = 0;
    STDMETHOD(GetFontVariant)(PDKFontVariantType peFontVariant) = 0;

    STDMETHOD(SetFontWeight)(DKFontWeightType eFontWeight) = 0;
    STDMETHOD(GetFontWeigth)(PDKFontWeightType peFontWeigth) = 0;

    STDMETHOD(SetShape)(SPtr<DKShape> spShape) = 0;
    STDMETHOD(GetShape)(SPtr<DKShape>& spShape) = 0;

    STDMETHOD(SetCursor)(DKCursorType eCursor) = 0;
    STDMETHOD(GetCursor)(PDKCursorType peCurspr) = 0;

    STDMETHOD(SetDisplay)(DKDisplayType eDisplay) = 0;
    STDMETHOD(GetDisplay)(PDKDisplayType peDisplay) = 0;

    STDMETHOD(SetFloat)(DKFloatType eFloat) = 0;
    STDMETHOD(GetFloat)(PDKFloatType peFloat) = 0;

    STDMETHOD(SetStartIndex)(UINT uStartIndex) = 0;
    STDMETHOD(GetStartIndex)(PUINT puStartIndex) = 0;

    STDMETHOD(SetEndIndex)(UINT uEndIndex) = 0;
    STDMETHOD(GetEndIndex)(PUINT puEndIndex) = 0;

    STDMETHOD(SetType)(DKAttributeType Type) = 0;
    STDMETHOD(GetType)(DKAttributeType *pType) = 0;

    STDMETHOD(CloneAttr)(IAttribute **ppAttr) = 0;
};

}
#endif


