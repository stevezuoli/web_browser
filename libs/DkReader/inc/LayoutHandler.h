#ifndef __LAYOUTHANDLER_INCLUDE__
#define __LAYOUTHANDLER_INCLUDE__

#include "dkObjBase.h"
#include "TextSmartLayout.h"

namespace DkFormat
{
    class ILayoutHandler
    {
    public:
        virtual ~ILayoutHandler(){};
        ILayoutHandler(){};

        STDMETHOD(GetLineSpacing)(PUINT puLineSpacing)=0;
        STDMETHOD(SetLineSpacing)(UINT uLineSpacing)=0;

        STDMETHOD(GetParaSpacing)(PUINT puParaSpacing)=0;
        STDMETHOD(SetParaSpacing)(UINT uParaSpacing)=0;

        STDMETHOD(GetIndent)(PINT piIndent)=0;
        STDMETHOD(SetIndent)(INT iIndent)=0;

        STDMETHOD(UpdateLayoutStyle)(UINT uLineSpacing, UINT uParaSpacing, INT iIndent)=0;

        STDMETHOD(SetSmartLayoutMode)(DKSmartLayoutMode eLayoutMode)=0;
        STDMETHOD(GetSmartLayoutMode)(PDKSmartLayoutMode peLayoutMode)=0;
    };
}

#endif

