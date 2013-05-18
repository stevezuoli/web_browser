#ifndef __DKSPTR_INCLUDED__
#define __DKSPTR_INCLUDED__

namespace DkFormat
{
    template<typename Lock> class RefCounter
    {
    public:
        RefCounter()
            : m_iCounter(0)
        {
            // do nothing
        }

        operator int()
        {
            return m_iCounter;
        }

        int operator++()
        {
            _lock.lock();
            int value = ++m_iCounter;
            _lock.unlock();
            return value;
        }

        int operator--()
        {
            _lock.lock();
            int value = --m_iCounter;
            _lock.unlock();
            return value;
        }

    private:
        int m_iCounter;
        Lock _lock;
    };

    struct SingleThread
    {
        void lock()
        {
            // do nothing
        }
        ;
        void unlock()
        {
            // do nothing
        }
        ;
    };

    template<typename T, typename ThreadLock = SingleThread> class SPtr
    {
    public:
        SPtr() 
            : m_Ptr(NULL), m_pCounter(NULL)
        {
            // do nothing
        }

          // P makes this smart ptr be able to accept pointer of its derived class
        template<typename P> explicit SPtr(P* ptr)
            : m_Ptr(NULL), m_pCounter(NULL)
        {
            if (NULL != ptr)
            {
                create( new RefCounter<ThreadLock>(), ptr);
            }
        }

        SPtr(const SPtr<T>& otherSmartPtr)
        {
            create(otherSmartPtr.m_pCounter, otherSmartPtr.m_Ptr);
        }

        const SPtr<T>& operator=(const SPtr<T>& otherSmartPtr)
        {
            if (&otherSmartPtr != this)
            {
                destory();
                create(otherSmartPtr.m_pCounter, otherSmartPtr.m_Ptr);
            }

            return *this;
        }

        ~SPtr()
        {
            destory();
        }

        T* operator->() const
        {
            return m_Ptr;
        }

        operator bool() const
        {
            return (m_Ptr != NULL);
        }

        T* Get() const
        {
            return m_Ptr;
        }

        void Clear()
        {
            destory();
        }

    private:
        // P makes this smart ptr be able to accept pointer of its derived class
        template<typename P> void create(RefCounter<ThreadLock> * pCounter, P* ptr)
        {
            m_pCounter = pCounter;
            if (NULL != m_pCounter)
            {
                ++*m_pCounter;
            }
            m_Ptr = ptr;
        }

        void destory()
        {
            if (NULL != m_Ptr && NULL != m_pCounter && 0 == --*m_pCounter)
            {
                delete m_pCounter;
                delete m_Ptr;
            }
            m_pCounter = NULL;
            m_Ptr = NULL;
        }

    private:
        // to hold the real pointer
        T * m_Ptr;
        // reference counter for m_Ptr
        RefCounter<ThreadLock> *m_pCounter;
    };
}

#endif // __DKSPTR_INCLUDED__

