#ifndef __CHARSETENCODING_INCLUDED__
#define __CHARSETENCODING_INCLUDED__

namespace DkFormat
{
    typedef enum
    {
        DK_CHARSET_ACSII,
        DK_CHARSET_UTF8,
        DK_CHARSET_UTF32_LE,
        DK_CHARSET_UNSUPPORTED,    // TODO: consider remove Unsupported
        DK_CHARSET_UNKNOWN,
    } CharsetEncoding;
}

#endif        // #ifndef __CHARSETENCODING_INCLUDED__

