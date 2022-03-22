// #include "time_thread.h"
#include <sys/time.h>

#include <stdlib.h>
#include <stdio.h>

#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

int gDebugTimeCached_time = 0;
int gDebugTimeCached_clock_gettime = 0;
int gDebugTimeCached_gettimeofday = 0;

// 提供秒级时间，
// 可用于避免多个线程重复调用time(NULL)
class CTimeThread
{
public:
    CTimeThread();
    ~CTimeThread();

    int64_t time_cached(time_t *__timer);
    int64_t get_milliseconds();
    bool start(uint32_t interval_milliseconds);
    void run();

    int gettimeofday_cached(timeval *tv, struct timezone *tz)
    {
        if (tv)
            *tv = _tv;
        if (tz)
            *tz = _tz;
        return 0;
    }

    int clock_gettime_cached(clockid_t clock_id, struct timespec *tp)
    {
        if (!tp)
            return -1;
        if (CLOCK_MONOTONIC == clock_id)
        {
            *tp = tp_monotonic;
            return 0;
        }
        else if (CLOCK_REALTIME == clock_id)
        {
            *tp = tp_realtime;
            return 0;
        }
        return ::clock_gettime(clock_id, tp);
    }

private:
    struct timeval _tv;
    struct timezone _tz;

    timespec tp_monotonic;
    timespec tp_realtime;

    int64_t _seconds;
    int64_t _milliseconds;

    uint32_t _interval_milliseconds;

    pthread_t _thread;
};

template <typename TYPE, void (TYPE::*Run)()>
void *thread_rounter(void *param)
{
    TYPE *p = (TYPE *)param;
    p->run();
    return NULL;
}

CTimeThread::CTimeThread() : _interval_milliseconds(1)
{
    ::gettimeofday(&_tv, NULL);

    _seconds = static_cast<int64_t>(_tv.tv_sec);
    _milliseconds = static_cast<int64_t>(_tv.tv_usec);

    ::clock_gettime(CLOCK_MONOTONIC, &tp_monotonic);
    ::clock_gettime(CLOCK_REALTIME, &tp_realtime);

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, 20);

    int errcode = pthread_create(&_thread, &attr,
                                 thread_rounter<CTimeThread, &CTimeThread::run>,
                                 (void *)this);
    if (errcode != 0)
    {
        printf("pthread_create error!");
    }
}

CTimeThread::~CTimeThread() {}

int64_t CTimeThread::time_cached(time_t *__timer)
{
    if (!__timer)
    {
        return _seconds;
    }
    *__timer = (time_t)_seconds;

    return _seconds;
}

void CTimeThread::run()
{
    while (true)
    {
        ::gettimeofday(&_tv, &_tz);
        // TODO merge two clock_gettime to once syscall
        ::clock_gettime(CLOCK_MONOTONIC, &tp_monotonic);
        ::clock_gettime(CLOCK_REALTIME, &tp_realtime);

        _seconds = static_cast<int64_t>(_tv.tv_sec);
        _milliseconds = static_cast<int64_t>(_tv.tv_usec);

        // printf("_seconds:%d\n",_seconds);

        struct timespec ts;
        ts.tv_sec = _interval_milliseconds / 1000;
        ts.tv_nsec = (_interval_milliseconds % 1000) * 1000000;
        while ((-1 == nanosleep(&ts, &ts)))
            ;
    }
}

CTimeThread *gCTimeThread = new CTimeThread();

/* Return the current time and put it in *TIMER if TIMER is not NULL.  */
unsigned int time_cached(time_t *__timer)
{
    if (gDebugTimeCached_time)
    {

        if (!__timer)
            return 0;
        return (unsigned int)(gCTimeThread->time_cached(__timer));
    }

    return ((unsigned int)::time(__timer));
};

int gettimeofday_cached(struct timeval *__tv, __timezone_ptr_t __tz)
{
    if (gDebugTimeCached_gettimeofday)
    {
        return gCTimeThread->gettimeofday_cached(__tv, __tz);
    }
    else
    {
        return ::gettimeofday(__tv, __tz);
    }
}

int clock_gettime_cached(clockid_t __clock_id, struct timespec *__tp)
{
    if (gDebugTimeCached_clock_gettime)
    {
        return gCTimeThread->clock_gettime_cached(__clock_id, __tp);
    }
    else
    {
        return ::clock_gettime(__clock_id, __tp);
    }
}

int main(int argc, char **argv)
{

    // 取得当前时间（单位：秒）

    printf("sleep ...\r\n");
    time_t pTime;

    timeval tv;
    struct timezone tz;
    struct timespec ts1;
    struct timespec ts2;
    // sleep(1);
    printf("start ...\r\n");

    // for (int i = 0; i < 1000 * 10000; i++)
    for (int i = 0; i < 1000 * 10000; i++)
    {
        if (i % 500000 == 0)
        {
            printf("i:%d \r\n", i);
        }

        auto t1 = time_cached(&pTime);

        auto t2 = gettimeofday_cached(&tv, &tz);

        auto t3 = clock_gettime_cached(CLOCK_REALTIME, &ts1);

        auto t4 = clock_gettime_cached(CLOCK_MONOTONIC, &ts2);

        // printf("==============================\r\n");
        printf("time_cached: %u\r\n", t1);
        printf("time_cached: %u\r\n", pTime);
        printf("gettimeofday_cached: tv_sec:%d tv_usec:%d tz_minuteswest:%d tz_dsttime:%d \r\n",
               tv.tv_sec, tv.tv_usec, tz.tz_minuteswest, tz.tz_dsttime);
        printf("CLOCK_REALTIME clock_gettime_cached: %10ld.%03ld \r\n", (long)ts1.tv_sec, ts1.tv_nsec / 1000000);
        printf("CLOCK_MONOTONIC clock_gettime_cached: %10ld.%03ld \r\n", (long)ts2.tv_sec, ts2.tv_nsec / 1000000);

        struct timespec ts;
        ts.tv_sec = 0;
        ts.tv_nsec = (100) * 1000000;
        while ((-1 == nanosleep(&ts, &ts)))
            ;
    }

    return 0;
}
