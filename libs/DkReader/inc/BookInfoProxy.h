#ifndef __BOOKINFOPROXY_INCLUDED__
#define __BOOKINFOPROXY_INCLUDED__

#include "dkBaseType.h"
#include "dkObjBase.h"
#include "dkBookSource.h"
#include "Bookmark.h"

namespace DkFormat
{
    // 书籍基本信息的Proxy类
    class IBookInfoProxy
    {
    public:
        IBookInfoProxy(void) {};
        virtual ~IBookInfoProxy(void) {};

        // 获取书籍信息接口.
        // 对于所有CHAR **的参数，只要返回值为S_OK，则表明函数内部已分配空间，需要调用者在外部释放空间
        STDMETHOD(GetBookName)(CHAR **ppBookName)=0;
        STDMETHOD(GetAuthorName)(CHAR **ppAuthorName)=0;
        STDMETHOD(GetCoverPagePath)(CHAR **ppCoverPagePath)=0;
        STDMETHOD(GetPublisherName)(CHAR **ppPublisherName)=0;
        STDMETHOD(GetISBN)(CHAR **ppISBN)=0;
        STDMETHOD(GetBookSource)(DkBookSource *pBookSource)=0;
        STDMETHOD(GetReadingProgress)(PUINT puProgress)=0;
        STDMETHOD(GetIsSerialized)(PBOOL pfIsSerialized)=0;
        STDMETHOD(GetSummary)(CHAR **ppSummary)=0;
        STDMETHOD(GetReadingBookmark)(IBookmark **ppBookmark)=0;
        STDMETHOD(GetRotation)(INT &iRotation)=0;
        //zhangjd add
        STDMETHOD(GetRecentOpenTime)(time_t &rTime)=0;//最近阅读时间
        STDMETHOD(GetFileSize)(UINT &ruSize)=0;//文件大小
        STDMETHOD(GetEmblodenLevel)(int &iLevel)=0;//加黑等级
        STDMETHOD(GetStripEdge)(BOOL &rbEnable)=0;
        STDMETHOD(GetFontName)(CHAR **ppFontName)=0;//当前书使用字体
        STDMETHOD(GetPassword)(char **ppPassword)=0;//返回密码


        STDMETHOD(SetBookName)(const CHAR *pBookName, UINT uLength)=0;
        STDMETHOD(SetAuthorName)(const CHAR *pAuthorName, UINT uLength)=0;
        STDMETHOD(SetCoverPagePath)(const CHAR *pCoverPagePath, UINT uLength)=0;
        STDMETHOD(SetPublisherName)(const CHAR *pPublisherName, UINT uLength)=0;
        STDMETHOD(SetISBN)(const CHAR *pISBN, UINT uLength)=0;
        STDMETHOD(SetReadingProgress)(UINT uProgress)=0;
        STDMETHOD(SetBookSource)(DkBookSource bookSource)=0;
        STDMETHOD(SetIsSerialized)(BOOL fIsSerialized)=0;
        STDMETHOD(SetSummary)(const CHAR *pSummary, UINT uLength)=0;
        STDMETHOD(SetReadingBookmark)(IBookmark *pBookmark)=0;
        STDMETHOD(SetRotation)(INT iRotation)=0;
        //zhangjd add
        STDMETHOD(SetRecentOpenTime)(time_t iTime)=0;//最近阅读时间
        STDMETHOD(SetFileSize)(UINT uSize)=0;//文件大小
        STDMETHOD(SetEmblodenLevel)(int iLevel)=0;//加黑等级
        STDMETHOD(SetStripEdge)(BOOL bEnable)=0;
        STDMETHOD(SetFontName)(const CHAR *pFontName)=0;//当前书使用字体
        STDMETHOD(SetPassword)(char *pPassword)=0;//保存密码

    };
}
#endif // __BOOKINFOPROXY_INCLUDED__
