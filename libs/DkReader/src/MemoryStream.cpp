#include <string.h>
#include "MemoryStream.h"
#include "dkWinError.h"

using namespace DkFormat;


CMemoryStream::CMemoryStream(SPtr<DkBufferBase> spBuf, DkStreamMode streamMode)
    : m_lPos(0), m_streamMode(streamMode), m_spBuffer(spBuf)
{
    // do nothing
}


CMemoryStream::~CMemoryStream(void)
{
    // do nothing
}

HRESULT CMemoryStream::Read(
        PBYTE8 pbBuffer, 
        INT iCount,
        PINT piReadCount
    )
{
    if (!this->m_spBuffer || NULL == this->m_spBuffer->pbBuffer)
    {
        return E_OBJ_NO_INIT;
    }

    if (this->m_streamMode != DSM_READ_ONLY && 
        this->m_streamMode != DSM_READ_WRITE)
    {
        return E_FAIL;
    }

    if (NULL == pbBuffer || iCount <= 0)
    {
        return E_INVALIDARG;
    }

    INT iRemainingCount = this->m_spBuffer->nLength - this->m_lPos;
    INT iReadCount = iCount < iRemainingCount ? iCount : iRemainingCount;
    if (iReadCount > 0)
    {
        memcpy(pbBuffer, this->m_spBuffer->pbBuffer + this->m_lPos, iReadCount);
    }

    if (piReadCount != NULL)
    {
        *piReadCount = iReadCount;
    }

    this->m_lPos += iReadCount;

    return S_OK;
}

HRESULT CMemoryStream::ReadByte(
        PINT piByte
    )
{
    if (!this->m_spBuffer || NULL == this->m_spBuffer->pbBuffer)
    {
        return E_OBJ_NO_INIT;
    }

    if (this->m_streamMode != DSM_READ_ONLY && 
        this->m_streamMode != DSM_READ_WRITE)
    {
        return E_FAIL;
    }

    if (NULL == piByte)
    {
        return E_INVALIDARG;
    }

    if (this->m_lPos == LONG(this->m_spBuffer->nLength))
    {
        // reach the end
        *piByte = -1;
        return E_FAIL;
    }

    *piByte = *(this->m_spBuffer->pbBuffer + this->m_lPos++);
    return S_OK;
}

HRESULT CMemoryStream::Seek(LONG lOffset, SeekMode origin, PLONG plPos)
{
    if (!this->m_spBuffer || NULL == this->m_spBuffer->pbBuffer)
    {
        return E_OBJ_NO_INIT;
    }

    LONG lPos = 0;
    UINT nLength = this->m_spBuffer->nLength;
    switch (origin)
    {
    case DK_SEEK_SET:
        lPos = lOffset;
        break;
    case DK_SEEK_CUR:
        lPos = this->m_lPos + lOffset;
        break;
    case DK_SEEK_END:
        lPos = nLength + lOffset;
        break;
    default:
        return E_INVALIDARG;
    }

    if (lPos < 0)
    {
        lPos = 0;
    }
    else if (lPos > LONG(nLength))
    {
        lPos = nLength;
    }

    this->m_lPos = lPos;
    if (plPos != NULL)
    {
        *plPos = lPos;
    }

    return S_OK;
}

HRESULT CMemoryStream::GetCanRead(PBOOL pfCanRead)
{
    if (!this->m_spBuffer || NULL == this->m_spBuffer->pbBuffer)
    {
        return E_OBJ_NO_INIT;
    }

    if (pfCanRead != NULL)
    {
        *pfCanRead = (DSM_READ_ONLY == this->m_streamMode || DSM_READ_WRITE == this->m_streamMode);
    }

    return S_OK;
}

HRESULT CMemoryStream::GetCanSeek(PBOOL pfCanSeek)
{
    if (!this->m_spBuffer || NULL == this->m_spBuffer->pbBuffer)
    {
        return E_OBJ_NO_INIT;
    }

    if (pfCanSeek != NULL)
    {
        *pfCanSeek = TRUE;
    }

    return S_OK;
}

HRESULT CMemoryStream::GetCanWrite(PBOOL pfCanWrite)
{
    if (!this->m_spBuffer || NULL == this->m_spBuffer->pbBuffer)
    {
        return E_OBJ_NO_INIT;
    }

    if (pfCanWrite != NULL)
    {
        *pfCanWrite = (DSM_READ_WRITE == this->m_streamMode || DSM_WRITE_ONLY == this->m_streamMode);
    }

    return S_OK;
}

HRESULT CMemoryStream::GetLength(PLONG plLength)
{
    if (!this->m_spBuffer || NULL == this->m_spBuffer->pbBuffer)
    {
        return E_OBJ_NO_INIT;
    }

    if (plLength != NULL)
    {
        *plLength = this->m_spBuffer->nLength;
    }

    return S_OK;
}

HRESULT CMemoryStream::Tell(PLONG plPos)
{
    if (!this->m_spBuffer || NULL == this->m_spBuffer->pbBuffer)
    {
        return E_OBJ_NO_INIT;
    }

    if (plPos != NULL)
    {
        *plPos = this->m_lPos;
    }

    return S_OK;
}

HRESULT CMemoryStream::Close()
{
    if (!this->m_spBuffer || NULL == this->m_spBuffer->pbBuffer)
    {
        return E_OBJ_NO_INIT;
    }

    this->m_spBuffer.Clear();
    this->m_lPos = 0;

    return S_OK;
}

HRESULT CMemoryStream::Flush()
{
    if (!this->m_spBuffer || NULL == this->m_spBuffer->pbBuffer)
    {
        return E_OBJ_NO_INIT;
    }

    if (DSM_READ_ONLY == this->m_streamMode)
    {
        return E_FAIL;
    }

    return S_OK;
}

HRESULT CMemoryStream::Write(
        PBYTE8 pbBuffer, 
        INT iCount,
        PINT piWriteCount)
{
    if (!this->m_spBuffer || NULL == this->m_spBuffer->pbBuffer)
    {
        return E_OBJ_NO_INIT;
    }

    if (this->m_streamMode != DSM_WRITE_ONLY && 
        this->m_streamMode != DSM_READ_WRITE)
    {
        return E_FAIL;
    }

    if (NULL == pbBuffer || iCount <= 0)
    {
        return E_INVALIDARG;
    }

    INT iRemainingCount = this->m_spBuffer->nLength - this->m_lPos;
    INT iWriteCount = iCount < iRemainingCount ? iCount : iRemainingCount;
    memcpy(this->m_spBuffer->pbBuffer + this->m_lPos, pbBuffer, iWriteCount);
    if (piWriteCount != NULL)
    {
        *piWriteCount = iWriteCount;
    }

    this->m_lPos += iWriteCount;

    return S_OK;
}

HRESULT CMemoryStream::WriteByte(
        BYTE8 bByte
    )
{
    if (!this->m_spBuffer || NULL == this->m_spBuffer->pbBuffer)
    {
        return E_OBJ_NO_INIT;
    }

    if (this->m_streamMode != DSM_WRITE_ONLY && 
        this->m_streamMode != DSM_READ_WRITE)
    {
        return E_FAIL;
    }

    if (this->m_lPos == LONG(this->m_spBuffer->nLength))
    {
        // reach the end
        return E_FAIL;
    }

    *(this->m_spBuffer->pbBuffer + this->m_lPos++) = bByte;
    return S_OK;
}

