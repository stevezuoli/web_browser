#ifndef __CMEMORYSTREAM_INCLUDED__
#define __CMEMORYSTREAM_INCLUDED__

#include "SequentialStream.h"
#include "DkSPtr.h"
#include "dkBuffer.h"

namespace DkFormat
{
    class CMemoryStream : public IDKSequentialStream
    {
    public:
        CMemoryStream(SPtr<DkBufferBase> spBuf, DkStreamMode streamMode);
        virtual ~CMemoryStream(void);

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
        LONG m_lPos;
        DkStreamMode m_streamMode;
        SPtr<DkBufferBase> m_spBuffer;
    };
}

#endif // __CMEMORYSTREAM_INCLUDED__

