#ifndef    __DKCOLOR_INCLUDED__
#define    __DKCOLOR_INCLUDED__

#include "dkBaseType.h"

namespace DkFormat
{
    typedef    struct DK_COLOR
    {
        BYTE8    red;
        BYTE8    green;
        BYTE8    blue;
        BYTE8    alpha;
    } DKColor, *PDKColor, **PPDKColor;

    class DKColors
    {
    public:
        static const DKColor red;
        static const DKColor green;
        static const DKColor blue;
        static const DKColor black;
        static const DKColor white;
    };
}
#endif

