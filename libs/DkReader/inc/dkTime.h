#ifndef _DKREADER_COMMON_DKTiME_H_
#define _DKREADER_COMMON_DKTiME_H_
#include <sys/time.h>
#include <stdlib.h>
namespace DkFormat
{

const int USEC_PER_MSEC = 1000;
const int MSEC_PER_SEC = 1000;
const int USEC_PER_SEC = 1000000;
const int NSEC_PER_USEC = 1000;

inline void NormalizeTimeval(struct timeval* tv)
{
    if (NULL ==  tv)
        return;
    
    if (tv->tv_usec >= USEC_PER_SEC || tv->tv_usec <= -USEC_PER_SEC)
    {
        tv->tv_sec += tv->tv_usec / USEC_PER_SEC;
        tv->tv_usec = tv->tv_usec % USEC_PER_SEC;
    }
    if (tv->tv_usec > 0 && tv->tv_sec < 0)
    {
        ++tv->tv_sec;
        tv->tv_usec -= USEC_PER_SEC;
    }
    if (tv->tv_usec < 0 && tv->tv_sec > 0)
    {
        --tv->tv_sec;
        tv->tv_usec += USEC_PER_SEC;
    }
}

class DkTime;

class DkTimeSpan
{
public:
    DkTimeSpan()
    {
    }
    static DkTimeSpan FromMilliSeconds(long ms)
    {
        struct timeval tv;
       
        tv.tv_sec = ms / MSEC_PER_SEC;
        tv.tv_usec = ms % MSEC_PER_SEC * USEC_PER_MSEC;
        return DkTimeSpan(tv);
    }
    static DkTimeSpan FromUSeconds(long us)
    {
        struct timeval tv;
        tv.tv_sec = us / USEC_PER_SEC;
        tv.tv_usec = us % USEC_PER_SEC;
        return DkTimeSpan(tv);
    }
    static DkTimeSpan FromSeconds(long seconds)
    {
        struct timeval tv;
        tv.tv_sec = seconds;
        tv.tv_usec = 0;
        return DkTimeSpan(tv);
    }
    long TotalMilliSeconds() const
    {
        return m_tv.tv_sec * MSEC_PER_SEC + m_tv.tv_usec / USEC_PER_MSEC;
    }
    long TotalSeconds() const
    {
        return m_tv.tv_sec;
    }

    friend DkTimeSpan operator-(const DkTimeSpan& lhs, const DkTimeSpan& rhs);
    friend DkTime operator+(const DkTime& lhs, const DkTimeSpan& rhs);
    friend DkTime operator-(const DkTime& lhs, const DkTimeSpan& rhs);

    friend bool operator<(const DkTimeSpan& lhs, const DkTimeSpan& rhs);
    friend bool operator<=(const DkTimeSpan& lhs, const DkTimeSpan& rhs);
    friend bool operator>(const DkTimeSpan& lhs, const DkTimeSpan& rhs);
    friend bool operator>=(const DkTimeSpan& lhs, const DkTimeSpan& rhs);
    friend bool operator==(const DkTimeSpan& lhs, const DkTimeSpan& rhs);
    friend bool operator!=(const DkTimeSpan& lhs, const DkTimeSpan& rhs);
private:
    DkTimeSpan(const struct timeval& tv)
    {
        m_tv = tv;
        NormalizeTimeval(&m_tv);
    }
    struct timeval m_tv;
};

class DkTime
{
public:
    DkTime()
    {
        gettimeofday(&m_tv, NULL);
    }
    DkTime(const timeval& tv)
    {
        m_tv = tv;
        NormalizeTimeval(&m_tv);
    }
    static DkTime Now()
    {
        DkTime t;
        gettimeofday(&t.m_tv, NULL);
        return t;
    }
    void MakeInvalid()
    {
        m_tv.tv_sec = -1;
        m_tv.tv_usec = -1;
    }
    void MakeNow()
    {
        gettimeofday(&m_tv, NULL);
    }

    bool IsValid() const
    {
        return m_tv.tv_sec >= 0
            && m_tv.tv_usec >= 0;
    }
    struct timespec ToTimeSpec() const
    {
        struct timespec ts;
        ts.tv_sec = m_tv.tv_sec;
        ts.tv_nsec = m_tv.tv_usec * NSEC_PER_USEC;
        return ts;
    }

    friend DkTimeSpan operator-(const DkTime& lhs, const DkTime& rhs);
    friend DkTime operator+(const DkTime& lhs, const DkTimeSpan& rhs);
    friend DkTime operator-(const DkTime& lhs, const DkTimeSpan& rhs);

    friend bool operator<(const DkTime& lhs, const DkTime& rhs);
    friend bool operator<=(const DkTime& lhs, const DkTime& rhs);
    friend bool operator>(const DkTime& lhs, const DkTime& rhs);
    friend bool operator>=(const DkTime& lhs, const DkTime& rhs);
    friend bool operator==(const DkTime& lhs, const DkTime& rhs);
    friend bool operator!=(const DkTime& lhs, const DkTime& rhs);

private:
    struct timeval m_tv;

};

inline DkTimeSpan operator-(const DkTime& lhs, const DkTime& rhs)
{
    return DkTimeSpan::FromUSeconds((lhs.m_tv.tv_sec - rhs.m_tv.tv_sec) * USEC_PER_SEC + (lhs.m_tv.tv_usec - rhs.m_tv.tv_usec));
}

inline DkTime operator+(const DkTime& lhs, const DkTimeSpan& rhs)
{
    struct timeval tv;
    tv.tv_sec =  lhs.m_tv.tv_sec + rhs.m_tv.tv_sec;
    tv.tv_usec = lhs.m_tv.tv_usec + rhs.m_tv.tv_usec;
    return DkTime(tv);
}

inline DkTime operator+(const DkTimeSpan& lhs, const DkTime& rhs)
{
    return rhs + lhs;
}
inline DkTime operator-(const DkTime& lhs, const DkTimeSpan& rhs)
{
    struct timeval tv;
    tv.tv_sec =  lhs.m_tv.tv_sec - rhs.m_tv.tv_sec;
    tv.tv_usec = lhs.m_tv.tv_usec - rhs.m_tv.tv_usec;
    return DkTime(tv);
}

inline DkTimeSpan operator-(const DkTimeSpan& lhs, const DkTimeSpan& rhs)
{
    struct timeval tv;
    tv.tv_sec =  lhs.m_tv.tv_sec - rhs.m_tv.tv_sec;
    tv.tv_usec = lhs.m_tv.tv_usec - rhs.m_tv.tv_usec;
    return DkTimeSpan(tv);
}
inline bool operator<(const DkTime& lhs, const DkTime& rhs)
{
    return lhs.m_tv.tv_sec < rhs.m_tv.tv_sec
        || (lhs.m_tv.tv_sec == rhs.m_tv.tv_sec
                && lhs.m_tv.tv_usec < rhs.m_tv.tv_usec);
}
inline bool operator<=(const DkTime& lhs, const DkTime& rhs)
{
    return lhs.m_tv.tv_sec < rhs.m_tv.tv_sec
        || (lhs.m_tv.tv_sec == rhs.m_tv.tv_sec
                && lhs.m_tv.tv_usec <= rhs.m_tv.tv_usec);
}
inline bool operator==(const DkTime& lhs, const DkTime& rhs)
{
    return lhs.m_tv.tv_sec == rhs.m_tv.tv_sec
                && lhs.m_tv.tv_usec == rhs.m_tv.tv_usec;
}
inline bool operator>(const DkTime& lhs, const DkTime& rhs)
{
    return !(lhs <= rhs);
}
inline bool operator>=(const DkTime& lhs, const DkTime& rhs)
{
    return !(lhs < rhs);
}
inline bool operator!=(const DkTime& lhs, const DkTime& rhs)
{
    return !(lhs == rhs);
}

inline bool operator<(const DkTimeSpan& lhs, const DkTimeSpan& rhs)
{
    return lhs.m_tv.tv_sec < rhs.m_tv.tv_sec
        || (lhs.m_tv.tv_sec == rhs.m_tv.tv_sec
                && lhs.m_tv.tv_usec < rhs.m_tv.tv_usec);
}
inline bool operator<=(const DkTimeSpan& lhs, const DkTimeSpan& rhs)
{
    return lhs.m_tv.tv_sec < rhs.m_tv.tv_sec
        || (lhs.m_tv.tv_sec == rhs.m_tv.tv_sec
                && lhs.m_tv.tv_usec <= rhs.m_tv.tv_usec);
}
inline bool operator==(const DkTimeSpan& lhs, const DkTimeSpan& rhs)
{
    return lhs.m_tv.tv_sec == rhs.m_tv.tv_sec
                && lhs.m_tv.tv_usec == rhs.m_tv.tv_usec;
}
inline bool operator>(const DkTimeSpan& lhs, const DkTimeSpan& rhs)
{
    return !(lhs <= rhs);
}
inline bool operator>=(const DkTimeSpan& lhs, const DkTimeSpan& rhs)
{
    return !(lhs < rhs);
}
inline bool operator!=(const DkTimeSpan& lhs, const DkTimeSpan& rhs)
{
    return !(lhs == rhs);
}

}

#endif
