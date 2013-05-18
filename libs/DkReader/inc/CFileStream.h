#ifndef __CFILESTREAM_INCLUDED__
#define __CFILESTREAM_INCLUDED__

#include "SequentialStream.h"
#include <stdio.h>

namespace DkFormat
{
    class CFileStream : public IDKSequentialStream
    {
    public:
        CFileStream(const CHAR *pFilePathName, DkStreamMode eType);
        virtual ~CFileStream(void);

        STDMETHOD(Read)(
            PBYTE8 pbBuffer, 
            INT iCount,
            PINT piReadCount
            );
        STDMETHOD(ReadByte)(
            PINT piByte
            );

        STDMETHOD(Seek)(LONG lOffset, SeekMode origin, PLONG plPos);
        STDMETHOD(GetCanRead)(PBOOL pfCanRead);
        STDMETHOD(GetCanSeek)(PBOOL pfCanSeek);
        STDMETHOD(GetCanWrite)(PBOOL pfCanWrite);
        STDMETHOD(GetLength)(PLONG plLength);
        STDMETHOD(Tell)(PLONG plPos);

        STDMETHOD(Close)();

        STDMETHOD(Flush)();

        STDMETHOD(Write)(
            PBYTE8 pbBuffer, 
            INT iCount,
            PINT piWriteCount);
        STDMETHOD(WriteByte)(
            BYTE8 bByte
            );

    private:
        FILE *m_pFile;
        DkStreamMode m_eType;
    };
}

#endif // __CFILESTREAM_INCLUDED__

