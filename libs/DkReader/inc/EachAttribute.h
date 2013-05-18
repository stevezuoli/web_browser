#ifndef __EACHATTRIBUTE_INCLUDED__
#define __EACHATTRIBUTE_INCLUDED__

#include "dkAttribute.h"

namespace DkFormat
{

class CAttrbuteBase : public IAttribute
{
public:
    CAttrbuteBase(){}
    virtual ~CAttrbuteBase(){}

    STDMETHOD(SetBgcolor)(SPtr<DKColor> spBgcolor)
    {
        return E_NOTIMPL;
    }

    STDMETHOD(GetBgcolor)(SPtr<DKColor>& spBgColor)
    {
        return E_NOTIMPL;
    }

    STDMETHOD(SetFgcolor)(SPtr<DKColor> spFgcolor)
    {
        return E_NOTIMPL;
    }

    STDMETHOD(GetFgcolor)(SPtr<DKColor>& spFgcolor) 
    {
        return E_NOTIMPL;
    }

    STDMETHOD(SetLetterSpacing)(UINT uLetterSpacing)
    {
        return E_NOTIMPL;
    }

    STDMETHOD(GetLetterSpacing)(PUINT puLetterSpacing)
    {
        return E_NOTIMPL;
    }

    STDMETHOD(SetTextAlign)(DKAlignType eTextAlign)
    {
        return E_NOTIMPL;
    }

    STDMETHOD(GetTextAlign)(PDKAlignType peTextAlign)
    {
        return E_NOTIMPL;
    }

    STDMETHOD(SetUnderline)(DKUnderline eUnderline)
    {
        return E_NOTIMPL;
    }

    STDMETHOD(GetUnderline)(PDKUnderline peUnderline)
    {
        return E_NOTIMPL;
    }

    STDMETHOD(SetStrikeThrough)(BOOL fStrikeThrough)
    {
        return E_NOTIMPL;
    }

    STDMETHOD(GetStrikeThrough)(BOOL *pfStrikeThrough)
    {
        return E_NOTIMPL;
    }

    STDMETHOD(SetTextIndent)(UINT uIndent)
    {
        return E_NOTIMPL;
    }

    STDMETHOD(GetTextIndent)(PUINT puIndent)
    {
        return E_NOTIMPL;
    }

    STDMETHOD(SetFontFamily)(const CHAR *pFontFamily)
    {
        return E_NOTIMPL;
    }

    STDMETHOD(GetFontFamily)(PCHAR *ppFontFamily)
    {
        return E_NOTIMPL;
    }

    STDMETHOD(SetFontSize)(UINT uFontSize)
    {
        return E_NOTIMPL;
    }

    STDMETHOD(GetFontSize)(PUINT puFontSize) 
    {
        return E_NOTIMPL;
    }

    STDMETHOD(SetFontStretch)(DKFontStretchType eFontStretch) 
    {
        return E_NOTIMPL;
    }

    STDMETHOD(GetFontStretch)(PDKFontStretchType peFonttretch)
    {
        return E_NOTIMPL;
    }

    STDMETHOD(SetFontStyle)(DKFontStyleType eFontStyle)
    {
        return E_NOTIMPL;
    }

    STDMETHOD(GetFontStyle)(PDKFontStyleType peFontStyle)
    {
        return E_NOTIMPL;
    }

    STDMETHOD(SetFontVariant)(DKFontVariantType eFontVariant)
    {
        return E_NOTIMPL;
    }

    STDMETHOD(GetFontVariant)(PDKFontVariantType peFontVariant) 
    {
        return E_NOTIMPL;
    }

    STDMETHOD(SetFontWeight)(DKFontWeightType eFontWeight)
    {
        return E_NOTIMPL;
    }

    STDMETHOD(GetFontWeigth)(PDKFontWeightType peFontWeigth)
    {
        return E_NOTIMPL;
    }

    STDMETHOD(SetShape)(SPtr<DKShape> spShape)
    {
        return E_NOTIMPL;
    }

    STDMETHOD(GetShape)(SPtr<DKShape>& spShape)
    {
        return E_NOTIMPL;
    }

    STDMETHOD(SetCursor)(DKCursorType eCursor)
    {
        return E_NOTIMPL;
    }

    STDMETHOD(GetCursor)(PDKCursorType peCurspr)
    {
        return E_NOTIMPL;
    }

    STDMETHOD(SetDisplay)(DKDisplayType eDisplay)
    {
        return E_NOTIMPL;
    }

    STDMETHOD(GetDisplay)(PDKDisplayType peDisplay)
    {
        return E_NOTIMPL;
    }

    STDMETHOD(SetFloat)(DKFloatType eFloat)
    {
        return E_NOTIMPL;
    }

    STDMETHOD(GetFloat)(PDKFloatType peFloat)
    {
        return E_NOTIMPL;
    }

    STDMETHOD(SetStartIndex)(UINT uStartIndex)
    {
        this->m_uStartIndex = uStartIndex;
        return S_OK;
    }

    STDMETHOD(GetStartIndex)(PUINT puStartIndex)
    {
        if (NULL == puStartIndex)
        {
            return E_INVALIDARG;
        }

        *puStartIndex = this->m_uStartIndex;
        return S_OK;
    }

    STDMETHOD(SetEndIndex)(UINT uEndIndex)
    {
        this->m_uEndIndex = uEndIndex;
        return S_OK;
    }

    STDMETHOD(GetEndIndex)(PUINT puEndIndex)
    {
        if (NULL == puEndIndex)
        {
            return E_INVALIDARG;
        }

        *puEndIndex = this->m_uEndIndex;
        return S_OK;
    }

    STDMETHOD(SetType)(DKAttributeType Type)
    {
        this->m_AttrbuteType = Type;
        return S_OK;
    }

    STDMETHOD(GetType)(DKAttributeType *pType)
    {
        if (NULL == pType)
        {
            return E_INVALIDARG;
        }

        *pType = this->m_AttrbuteType;
        return S_OK;
    }

    STDMETHOD(CloneAttr)(IAttribute **ppAttr)
    {
        return E_NOTIMPL;
    }

protected:
    UINT m_uStartIndex;
    UINT m_uEndIndex;
    DKAttributeType m_AttrbuteType;
};

class CAttrBgcolor : public CAttrbuteBase
{
public:
    CAttrBgcolor(SPtr<DKColor> spBgcolor = SPtr<DKColor>(), UINT uStartIndex = 0, UINT uEndIndex = 0);
    virtual ~CAttrBgcolor();

    STDMETHOD(SetBgcolor)(SPtr<DKColor> spBgcolor);
    STDMETHOD(GetBgcolor)(SPtr<DKColor>& spBgcolor);
    STDMETHOD(CloneAttr)(IAttribute **ppAttr);

private:
    SPtr<DKColor> m_spBgColor;
};

class CAttrFgcolor : public CAttrbuteBase
{
public:
    CAttrFgcolor(SPtr<DKColor> spFgcolor = SPtr<DKColor>(), UINT uStartIndex = 0, UINT uEndIndex = 0);
    virtual ~CAttrFgcolor();

    STDMETHOD(SetFgcolor)(SPtr<DKColor> spFgcolor);
    STDMETHOD(GetFgcolor)(SPtr<DKColor>& spFgcolor);
    STDMETHOD(CloneAttr)(IAttribute **ppAttr);

private:
    SPtr<DKColor> m_spFgColor;
};

class CAttrLetterSpacing : public CAttrbuteBase
{
public:
    CAttrLetterSpacing(UINT uLetterSpacing = 1, UINT uStartIndex = 0, UINT uEndIndex = 0);
    virtual ~CAttrLetterSpacing(){};

    STDMETHOD(SetLetterSpacing)(UINT uLetterSpacing);
    STDMETHOD(GetLetterSpacing)(PUINT puLetterSpacing);
    STDMETHOD(CloneAttr)(IAttribute **ppAttr);

private:
    UINT m_uLetterSpacing;
};

class CAttrTextAlign : public CAttrbuteBase
{
public:
    CAttrTextAlign(DKAlignType eTextAlign = DK_ALIGN_LEFT, UINT uStartIndex=0, UINT uEndIndex=0);
    virtual ~CAttrTextAlign(){};

    STDMETHOD(SetTextAlign)(DKAlignType eTextAlign);
    STDMETHOD(GetTextAlign)(PDKAlignType peTextAlign);
    STDMETHOD(CloneAttr)(IAttribute **ppAttr);

private:
    DKAlignType m_eTextAlign;
};

class CAttrUnderline : public CAttrbuteBase
{
public:
    CAttrUnderline(DKUnderline eUnderline = DK_UNDERLINE_NONE, UINT uStartIndex=0, UINT uEndIndex=0);
    virtual ~CAttrUnderline(){};

    STDMETHOD(SetUnderline)(DKUnderline eUnderline);
    STDMETHOD(GetUnderline)(PDKUnderline peUnderline);
    STDMETHOD(CloneAttr)(IAttribute **ppAttr);

private:
    DKUnderline m_eUnderline;
};

class CAttrStrikeThrough : public CAttrbuteBase
{
public:
    CAttrStrikeThrough(BOOL fStrikeThrough = FALSE, UINT uStartIndex=0, UINT uEndIndex=0);
    virtual ~CAttrStrikeThrough(){};

    STDMETHOD(SetStrikeThrough)(BOOL fStrikeThrough);
    STDMETHOD(GetStrikeThrough)(BOOL *pfStrikeThrough);
    STDMETHOD(CloneAttr)(IAttribute **ppAttr);

private:
    BOOL m_fStrikeThrough;
};

class CAttrTextIndent : public CAttrbuteBase
{
public:
    CAttrTextIndent(UINT uIndent = 0, UINT uStartIndex=0, UINT uEndIndex=0);
    virtual ~CAttrTextIndent(){};

    STDMETHOD(SetTextIndent)(UINT uIndent);
    STDMETHOD(GetTextIndent)(PUINT puIndent);
    STDMETHOD(CloneAttr)(IAttribute **ppAttr);

private:
    UINT m_uTextIndent;
};

class CAttrFontFamily : public CAttrbuteBase
{
public:
    CAttrFontFamily(const CHAR *pFontFamily = NULL, UINT uStartIndex=0, UINT uEndIndex=0);
    virtual ~CAttrFontFamily()
    {
        if (this->m_pFontFamily != NULL)
        {
            delete[] this->m_pFontFamily;
            this->m_pFontFamily = NULL;
        }
    };

    STDMETHOD(SetFontFamily)(const CHAR *fontFamily);
    STDMETHOD(GetFontFamily)(PCHAR *ppFontFamily);
    STDMETHOD(CloneAttr)(IAttribute **ppAttr);

private:
    CHAR *m_pFontFamily;
};

class CAttrFontSize : public CAttrbuteBase
{
public:
    CAttrFontSize(UINT uFontSize = 20, UINT uStartIndex=0, UINT uEndIndex=0);
    virtual ~CAttrFontSize(){};

    STDMETHOD(SetFontSize)(UINT uFontSize);
    STDMETHOD(GetFontSize)(PUINT puFontSize);
    STDMETHOD(CloneAttr)(IAttribute **ppAttr);

private:
    UINT m_uFontSize;
};

class CAttrFontStretch : public CAttrbuteBase
{
public:
    CAttrFontStretch(DKFontStretchType eFontStretch = DK_STRETCH_NORMAL, UINT uStartIndex=0, UINT uEndIndex=0);
    virtual ~CAttrFontStretch(){};

    STDMETHOD(SetFontStretch)(DKFontStretchType eFontStretch);
    STDMETHOD(GetFontStretch)(PDKFontStretchType peFonttretch);
    STDMETHOD(CloneAttr)(IAttribute **ppAttr);

private:
    DKFontStretchType m_eFontStretch;
};

class CAttrFontStyle : public CAttrbuteBase
{
public:
    CAttrFontStyle(DKFontStyleType eFontStyle = DK_FONT_STYLE_NORMAL, UINT uStartIndex=0, UINT uEndIndex=0);
    virtual ~CAttrFontStyle(){};

    STDMETHOD(SetFontStyle)(DKFontStyleType eFontStyle);
    STDMETHOD(GetFontStyle)(PDKFontStyleType peFontStyle);
    STDMETHOD(CloneAttr)(IAttribute **ppAttr);

private:
    DKFontStyleType m_eFontStyle;
};

class CAttrFontVariant : public CAttrbuteBase
{
public:
    CAttrFontVariant(DKFontVariantType eFontVariant = DK_FONT_VARIANT_NOMAL, UINT uStartIndex=0, UINT uEndIndex=0);
    virtual ~CAttrFontVariant(){};

    STDMETHOD(SetFontVariant)(DKFontVariantType eFontVariant);
    STDMETHOD(GetFontVariant)(PDKFontVariantType peFontVariant);
    STDMETHOD(CloneAttr)(IAttribute **ppAttr);

private:
    DKFontVariantType m_eFontVariant;
};

class CAttrFontWeight : public CAttrbuteBase
{
public:
    CAttrFontWeight(DKFontWeightType eFontWeight = DK_WEIGHT_NORMAL, UINT uStartIndex=0, UINT uEndIndex=0);
    virtual ~CAttrFontWeight(){};

    STDMETHOD(SetFontWeight)(DKFontWeightType eFontWeight);
    STDMETHOD(GetFontWeigth)(PDKFontWeightType peFontWeigth);
    STDMETHOD(CloneAttr)(IAttribute **ppAttr);

private:
    DKFontWeightType m_eFontWeight;
};

class CAttrShape : public CAttrbuteBase
{
public:
    CAttrShape(SPtr<DKShape> spShape = SPtr<DKShape>(), UINT uStartIndex=0, UINT uEndIndex=0);
    virtual ~CAttrShape(){};

    STDMETHOD(SetShape)(SPtr<DKShape> spShape);
    STDMETHOD(GetShape)(SPtr<DKShape>& spShape);
    STDMETHOD(CloneAttr)(IAttribute **ppAttr);

private:
    SPtr<DKShape> m_spShape;
};

class CAttrCursor : public CAttrbuteBase
{
public:
    CAttrCursor(DKCursorType eCursor = DK_CURSOR_TEXT, UINT uStartIndex=0, UINT uEndIndex=0);
    virtual ~CAttrCursor(){};

    STDMETHOD(SetCursor)(DKCursorType eCursor);
    STDMETHOD(GetCursor)(PDKCursorType peCursor);
    STDMETHOD(CloneAttr)(IAttribute **ppAttr);

private:
    DKCursorType m_eCursor;
};

class CAttrDisplay : public CAttrbuteBase
{
public:
    CAttrDisplay(DKDisplayType eDisplay = DK_DISPLAY_NONE, UINT uStartIndex=0, UINT uEndIndex=0);
    virtual ~CAttrDisplay(){};

    STDMETHOD(SetDisplay)(DKDisplayType eDisplay);
    STDMETHOD(GetDisplay)(PDKDisplayType peDisplay);
    STDMETHOD(CloneAttr)(IAttribute **ppAttr);

private:
    DKDisplayType m_eDisplay;
};

class CAttrFloat : public CAttrbuteBase
{
public:
    CAttrFloat(DKFloatType eFloat = DK_FLOAT_NONE, UINT uStartIndex=0, UINT uEndIndex=0);
    virtual ~CAttrFloat(){};

    STDMETHOD(SetFloat)(DKFloatType eFloat);
    STDMETHOD(GetFloat)(PDKFloatType peFloat);
    STDMETHOD(CloneAttr)(IAttribute **ppAttr);

private:
    DKFloatType m_eFloat;
};

}

#endif

