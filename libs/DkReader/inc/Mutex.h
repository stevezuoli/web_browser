#ifndef __MUTEX__
#define __MUTEX__

#include <string>
#ifdef WIN32
#include <windows.h> 
#else
#include <sys/time.h>
#include <pthread.h>
#include <memory.h>
#include <stdlib.h>
#include <malloc.h>
static pthread_mutexattr_t  *sMutexAttr=NULL;
static void MutexAttrInit();
static pthread_once_t sMutexAttrInit = PTHREAD_ONCE_INIT;
#endif

#ifndef WIN32
void MutexAttrInit()
{
    sMutexAttr = (pthread_mutexattr_t*)malloc(sizeof(pthread_mutexattr_t));
    ::memset(sMutexAttr, 0, sizeof(pthread_mutexattr_t));
    pthread_mutexattr_init(sMutexAttr);
}
#endif

class Mutex;
class MutexBase
{
    friend class Mutex;

    void Lock(){
#ifdef WIN32
        ::EnterCriticalSection(&cs);
#else
        (void)pthread_mutex_lock(&cs);
#endif
    }

    void Unlock(){
#ifdef WIN32
        ::LeaveCriticalSection(&cs);
#else
        pthread_mutex_unlock(&cs);
#endif
    }
protected :
    MutexBase(){
#ifdef WIN32
        ::InitializeCriticalSection(&cs);
#else
        (void)pthread_once(&sMutexAttrInit, MutexAttrInit);
        (void)pthread_mutex_init(&cs, sMutexAttr);
#endif
    } 
    ~ MutexBase(){
#ifdef WIN32
        ::DeleteCriticalSection(&cs);
#else
        pthread_mutex_destroy(&cs);
#endif
    } 
protected:
#ifdef WIN32
    CRITICAL_SECTION cs;
#else
    pthread_mutex_t cs;
#endif
} ;

#ifndef WIN32
class AutoLock
{
public:
    AutoLock(pthread_mutex_t* lock)
    {
        m_lock = lock;
        if (NULL != m_lock)
        {
            pthread_mutex_lock(m_lock);
        }
    }
    ~AutoLock()
    {
        if (NULL != m_lock)
        {
            pthread_mutex_unlock(m_lock);
            m_lock = NULL;
        }
    }

private:
    pthread_mutex_t* m_lock;
    AutoLock(const AutoLock&);
    AutoLock& operator=(const AutoLock&);
};
#endif


class Mutex
{
    MutexBase * _pObj;
public :
    Mutex(MutexBase * const pObj){
        _pObj = pObj;
        if (NULL != _pObj)
            _pObj -> Lock();
    } 
    ~ Mutex(){
        if (NULL != _pObj)
            _pObj -> Unlock();
    } 
} ;
#endif
