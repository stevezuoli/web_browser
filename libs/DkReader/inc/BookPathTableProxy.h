#ifndef __BOOKPATHTABLEPROXY_INCLUDED__
#define __BOOKPATHTABLEPROXY_INCLUDED__

#include "dkBaseType.h"
#include "dkObjBase.h"

namespace DkFormat
{
    typedef struct 
    {
        UINT nFileId;        // 文件Id
        UINT nParentId;        // 所属父文件Id
        // TODO:define MAX_FILEPATHNAME
        CHAR rgPath[1024];        // 相对于父文件的相对路径
    } DkpPathItem;

    // PathTable的Proxy类
    class IBookPathTableProxy
    {
    public:
        IBookPathTableProxy() {};
        virtual ~IBookPathTableProxy() {};

        STDMETHOD(GetPathTable)(DkpPathItem **ppList, PUINT pnCount)=0;
        STDMETHOD(SetPathTable)(DkpPathItem *pList, UINT nCount)=0;

        //此接口只释放GetPathTable获取的资源
        STDMETHOD(ReleasePathTable)(DkpPathItem *pList)=0;
    };
}
#endif // __BOOKPATHTABLEPROXY_INCLUDED__

