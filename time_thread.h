
// Writed by yijian on 2019/2/27
#ifndef MOOON_SYS_TIME_THREAD_H
#define MOOON_SYS_TIME_THREAD_H

#include <time.h>
#include <stdint.h>
#include <pthread.h>
#include <sys/time.h>

// 提供秒级时间，
// 可用于避免多个线程重复调用time(NULL)
class CTimeThread
{
public:
    CTimeThread();
    ~CTimeThread();

    int64_t get_seconds() const;
    int64_t get_milliseconds() const;
    bool start(uint32_t interval_milliseconds);
    void run();
private:

    struct timeval _tv;
    struct timezone _tz;
    struct timespec _ts;

    int64_t _seconds;
    int64_t _milliseconds;

    uint32_t _interval_milliseconds;

    pthread_t _thread;
};

#endif // MOOON_SYS_TIME_THREAD_H
