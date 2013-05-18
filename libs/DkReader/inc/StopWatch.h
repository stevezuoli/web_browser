#ifndef _STOPWATCH_H
#define _STOPWATCH_H

#include "dkTime.h"
namespace DkFormat
{
class StopWatch
{
public:
    enum {INVALID_DURATION = -1};
    enum InitFlag {INIT_START = 1, INIT_STOP = 2};
    StopWatch(InitFlag initFlag = INIT_STOP)
    {
        m_running = false;
        m_durationInMs = INVALID_DURATION;
        if (INIT_START == initFlag)
        {
            Start();
        }
    }
    void Start()
    {
        if (m_running)
        {
            // Can't start a watch twice before stop
            return;
        }
        m_start = DkTime::Now();
        m_durationInMs = INVALID_DURATION;
        m_running = true;
    }

    bool IsRunning() const
    {
        return m_running;
    }

    /* return ms elapsed from start */
    int Stop()
    {
        if (m_running) 
        {
            m_stop = DkTime::Now();
            m_durationInMs = (m_stop - m_start).TotalMilliSeconds();
        }

        m_running = false;
        return m_durationInMs;
    }

    int DurationInMs() const
    {
        return m_durationInMs;
    }

private:
    DkTime m_start, m_stop;
    bool m_running;
    int m_durationInMs;
};
}
#endif
