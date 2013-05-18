
#ifndef __SEQUENTIALSTREAM_INCLUDED__
#define __SEQUENTIALSTREAM_INCLUDED__

#include "dkObjBase.h"

namespace DkFormat
{
    typedef enum
    {
        DK_SEEK_SET,
        DK_SEEK_CUR,
        DK_SEEK_END
    } SeekMode;

    typedef enum
    {
        DSM_READ_ONLY,
        DSM_WRITE_ONLY,
        DSM_READ_WRITE
    } DkStreamMode;

    class IDKSequentialStream
    {
    public:
        virtual ~IDKSequentialStream(){};
        IDKSequentialStream(){};

        // TODO: consider the async read/write???
        STDMETHOD(Read)(
            PBYTE8 pbBuffer, 
            INT iCount,
            PINT piReadCount
        )=0;
        STDMETHOD(ReadByte)(
            PINT piByte
        )=0;

        STDMETHOD(Seek)(LONG lOffset, SeekMode origin, PLONG plPos)=0;
        STDMETHOD(GetCanRead)(PBOOL pfCanRead)=0;
        STDMETHOD(GetCanSeek)(PBOOL pfCanSeek)=0;
        STDMETHOD(GetCanWrite)(PBOOL pfCanWrite)=0;
        STDMETHOD(GetLength)(PLONG plLength)=0;
        STDMETHOD(Tell)(PLONG plPos)=0;

        STDMETHOD(Close)()=0;

        STDMETHOD(Flush)()=0;

        STDMETHOD(Write)(
            PBYTE8 pbBuffer, 
            INT iCount,
            PINT piWriteCount)=0;
        STDMETHOD(WriteByte)(
            BYTE8 bByte
        )=0;
    };
}

#endif // def __SEQUENTIALSTREAM_INCLUDED__

