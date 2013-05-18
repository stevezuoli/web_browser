#ifndef __UTILITY_INCLUDED__
#define __UTILITY_INCLUDED__

#include "dkBaseType.h"
#include "CharsetEncoding.h"
#include <stdint.h>
#include <string>
//#include "TextLayout.h"

namespace DkFormat
{
    class CUtility
    {
    public:
        static std::string TransferTimeToString(const time_t _tTime)
        {
            char _tBuf[40] = {0};
            struct tm * newtime=localtime(&_tTime);
            strftime( _tBuf, sizeof(_tBuf), "%F %T", newtime);
            return std::string(_tBuf);
        }

        static time_t   TransferStringToTime(const std::string _strTime);        
        static std::string TrimString(const std::string istring)
        {
            const char* pWhiteSpace = " \n\t\r\0xb";
            std::string::size_type first = istring.find_first_not_of(pWhiteSpace);
            if (first == std::string::npos) {
                return "";
            }
            else
            {
                std::string::size_type last = istring.find_last_not_of(pWhiteSpace);
                return istring.substr( first, last - first + 1);
            }
        }

        static PCHAR ReplaceChar(PCHAR pSrcStr, const CHAR destSep, const CHAR srcSep);
        static DK_RECT GetIntersectRect(const DK_RECT &rect1, const DK_RECT &rect2);
        static BOOL IsEmptyRect(DK_RECT rect);
        static BOOL StringEndWith(LPCSTR lpszStr, LPCSTR lpszExt, BOOL fIgnoreCase = FALSE);
        static HRESULT ConvertEncoding(
            CharsetEncoding toEncoding,         // destination encoding
            CharsetEncoding fromEncoding,           // source encoding
            const char *pInBuf,                 // input buffer
            UINT ulInByteCount,                 // input buffer length
            char *pOutBuf,                      // output buffer
            UINT ulOutByteCount,                    // output buffer length
            PULONG pulConvertedByteCount,       // the number of converted bytes in output buffer
            BOOL fDiscardIllegal = TRUE,            // Indicates whether to discard illegal sequence in the conversion
            BOOL fEnableTransliterate = TRUE        // Indicates whether enable transliteration in the conversion
            );
        static BOOL CheckFileExistence(const CHAR *lpszFilename);
        static INT GetFileLength(const CHAR *lpszFilename);

        // the memory is new uint8_t[] and must be delete[] by caller
        static uint8_t* GetGB2312Buffer(size_t* gb2312InBytes);

        // the memory is new uint8_t[] and must be delete[] by caller
        static uint8_t* GetUTF8BufferForGB2312(size_t* utf8InBytes);

        static HRESULT UnpackArchive(const char *pArcPathName, const char *pOutPath, const char *pPassword = NULL);

        static BOOL ReadStringFromFile(std::string& str, FILE* fp);
        static BOOL WriteStringToFile(std::string str, FILE* fp);
        static BOOL ReadIntFromFile(int &iNum, FILE* fp);
        static BOOL WriteIntToFile(int iNum, FILE* fp);
        static BOOL WriteLongToFile(long lNum, FILE* fp);
        static BOOL ReadLongFromFile(long &lNum, FILE* fp);
    private:
        static const char * GetEncodingString(CharsetEncoding encoding);
    };
}

#endif  // ndef __UTILITY_INCLUDED__

