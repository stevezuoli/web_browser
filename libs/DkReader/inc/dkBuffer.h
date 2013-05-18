#ifndef    __DKBUFFER_INCLUDED__
#define    __DKBUFFER_INCLUDED__

#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include "dkBaseType.h"

#if ( RUN_PLATFORM == PLATFORM_LINUX )
    #include <sys/types.h>
    #include <sys/mman.h>
    #include <unistd.h>
#else
    #include <io.h>
#endif

namespace DkFormat
{
    typedef void (*FreeCallBack)(void *pMemory);

    typedef    struct DkBufferBase
    {
        BYTE8 *pbBuffer;
        UINT nLength;

        virtual ~DkBufferBase() {};
        // we don't allow to initialize this base directly.
    protected:
        DkBufferBase() {};
    } DkBufferBase;

    typedef    struct DkBuffer : public DkBufferBase
    {
    private:
        FreeCallBack pFreeMemFunc;

    public:
        DkBuffer()
        {
            pbBuffer = NULL;
            nLength = 0;
            pFreeMemFunc = NULL;
        }

        DkBuffer(BYTE8 *pbBuf, UINT nDataLength, FreeCallBack pFreeFunc = NULL)
        {
            pbBuffer = pbBuf;
            nLength = nDataLength;
            pFreeMemFunc = pFreeFunc;
        }

        virtual ~DkBuffer()
        {
            if (pbBuffer != NULL)
            {
                if(NULL != pFreeMemFunc)
                {
                    pFreeMemFunc(pbBuffer);
                }
                else
                {
                    delete[] pbBuffer;
                }
                pbBuffer = NULL;
            }

            nLength = 0;
        }

    } DkBuffer;

    typedef    struct DkMappedBuffer : public DkBufferBase
    {
        BOOL fMappedSuccessfully;

        DkMappedBuffer()
        {
            pbBuffer = NULL;
            nLength = 0;
            fMappedSuccessfully = FALSE;
        }

        DkMappedBuffer(const CHAR *lpszFilePath)
        {
            pbBuffer = NULL;
            nLength = 0;
            fMappedSuccessfully = FALSE;

            if (NULL == lpszFilePath || 0 == lpszFilePath[0])
            {
                return;
            }

#if ( RUN_PLATFORM == PLATFORM_LINUX )
            int fd = open(lpszFilePath, O_RDONLY);
#else
            int fd = open(lpszFilePath, O_BINARY | O_RDONLY);
#endif
            if(fd < 0)
            {
                return;
            }

            struct stat fileStat;
            if (fstat(fd, &fileStat) == -1)
            {
                close(fd);
                return;
            }

            PBYTE8 pBuf = NULL;
#if ( RUN_PLATFORM == PLATFORM_LINUX )
            pBuf = (unsigned char*)mmap(NULL, fileStat.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
            if (MAP_FAILED == pBuf)
            {
                close(fd);
                return;
            }

#elif ( RUN_PLATFORM == PLATFORM_WINDOWS )
            // TODO: consider using memory mapping for opening file in the future
            pBuf = new BYTE8[fileStat.st_size];
            if (NULL == pBuf)
            {
                close(fd);
                return;
            }

            memset(pBuf, 0, fileStat.st_size);
            if (read(fd, pBuf, fileStat.st_size) != fileStat.st_size)
            {
                close(fd);
                delete[] pBuf;
                return;
            }
#else
#error Unsupported Platform
#endif
            close(fd);

            pbBuffer = pBuf;
            nLength = fileStat.st_size;
            fMappedSuccessfully = TRUE;
        }

        virtual ~DkMappedBuffer()
        {
            if (pbBuffer != NULL)
            {
#if ( RUN_PLATFORM == PLATFORM_LINUX )
                munmap(pbBuffer, nLength);
#else
                delete[] pbBuffer;
#endif
                pbBuffer = NULL;
            }

            nLength = 0;
            fMappedSuccessfully = FALSE;
        }
    } DkMappedBuffer;
}

#endif // __DKBUFFER_INCLUDED__


