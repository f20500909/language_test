#include <assert.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#include <chrono>
#include <mutex>
#include <thread>

int gDebugTimeCached_time = 1;
int gDebugTimeCached_clock_gettime = 1;
int gDebugTimeCached_gettimeofday = 1;

template <typename TYPE, void (TYPE::*Run)()>
void *thread_rounter(void *param) {
  TYPE *p = (TYPE *)param;
  p->run();
  return NULL;
}

void showPlatform() {
#ifdef __x86_64__
  printf("__x86_64__\r\n");
#elif __i386__
  printf("__i386__\r\n");
#endif
  printf("sizeof(__time_t): %d,sizeof(__time_t):%d\r\n", sizeof(__time_t),
         sizeof(__time_t));
}

// use cache to reduce system call in i386
class CTimeThread {
 public:
  CTimeThread();
  ~CTimeThread();

  void run();

  int64_t cached_time(time_t *__timer);

  int64_t get_milliseconds();

  int cached_gettimeofday(timeval *tv, struct timezone *tz);

  int cached_clock_gettime(clockid_t clock_id, struct timespec *tp);

 private:
  struct timeval _tv;
  struct timezone _tz;
  timespec _tp_monotonic;
  timespec _tp_realtime;
  int64_t _seconds;
  uint32_t _interval_milliseconds;

  uint64_t _time_data;
  pthread_t _thread;
};

CTimeThread::CTimeThread() : _interval_milliseconds(10) {
  ::gettimeofday(&_tv, NULL);

  _seconds = static_cast<int64_t>(_tv.tv_sec);

  ::clock_gettime(CLOCK_MONOTONIC, &_tp_monotonic);
  ::clock_gettime(CLOCK_REALTIME, &_tp_realtime);

  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

  int errcode = pthread_create(&_thread, &attr,
                               thread_rounter<CTimeThread, &CTimeThread::run>,
                               (void *)this);
  if (errcode != 0) {
    printf("!!!!!!!!!! pthread_create error!");
  }
}

CTimeThread::~CTimeThread() {}

int64_t CTimeThread::cached_time(time_t *__timer) {
  if (!__timer) {
    return _seconds;
  }
  *__timer = (time_t)_seconds;

  return _seconds;
}

void CTimeThread::run() {
  while (true) {
    ::gettimeofday(&_tv, &_tz);

    // TODO merge two clock_gettime to one syscall
    ::clock_gettime(CLOCK_MONOTONIC, &_tp_monotonic);

    // ::clock_gettime(CLOCK_REALTIME, &_tp_realtime);
    syscall(__NR_clock_gettime, CLOCK_MONOTONIC, &_tp_realtime);

    _seconds = static_cast<int64_t>(_tv.tv_sec);
    // printf("_seconds:%d\n",_seconds);

    struct timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = _interval_milliseconds;
    while ((-1 == nanosleep(&ts, &ts)))
      ;
  }
}

int CTimeThread::cached_gettimeofday(timeval *tv, struct timezone *tz) {
  if (tv) *tv = _tv;
  if (tz) *tz = _tz;
  return 0;
}

int CTimeThread::cached_clock_gettime(clockid_t clock_id, struct timespec *tp) {
  if (!tp) return -1;
  //   if (CLOCK_MONOTONIC == clock_id) {
  //     *tp = _tp_monotonic;
  //     return 0;
  //   } else if (CLOCK_REALTIME == clock_id) {
  //     *tp = _tp_realtime;
  //     return 0;
  //   }

  uint64_t prev_time =
      uint64_t(tp->tv_sec) * 1000 + uint64_t(tp->tv_nsec) / 1000000;

  while (true) {
    // Critical section start
    __sync_bool_compare_and_swap(&(tp->tv_sec), tp->tv_sec,
                                 _tp_realtime.tv_sec);
    __sync_bool_compare_and_swap(&(tp->tv_nsec), tp->tv_nsec,
                                 _tp_realtime.tv_nsec);

    // Critical section end
    uint64_t cur_time =
        uint64_t(tp->tv_sec) * 1000 + uint64_t(tp->tv_nsec) / 1000000;
    if (cur_time >= prev_time) {
      break;
    }
  }

  return 0;

  //   return ::clock_gettime(clock_id, tp);
}

CTimeThread *gCTimeThread = nullptr;

unsigned int cached_time(time_t *__timer) {
  assert(gCTimeThread);

  if (gDebugTimeCached_time) {
    if (!__timer) return 0;
    return (unsigned int)(gCTimeThread->cached_time(__timer));
  }

  return ((unsigned int)::time(__timer));
};

int cached_gettimeofday(struct timeval *__tv, __timezone_ptr_t __tz) {
  assert(gCTimeThread);
  if (gDebugTimeCached_gettimeofday) {
    return gCTimeThread->cached_gettimeofday(__tv, __tz);
  } else {
    return ::gettimeofday(__tv, __tz);
  }
}

int cached_clock_gettime(clockid_t __clock_id, struct timespec *__tp) {
  assert(gCTimeThread);
  if (gDebugTimeCached_clock_gettime) {
    return gCTimeThread->cached_clock_gettime(__clock_id, __tp);
  } else {
    return ::clock_gettime(__clock_id, __tp);
  }
}

int main() {
  showPlatform();

  gCTimeThread = new CTimeThread();
  std::this_thread::sleep_for(std::chrono::milliseconds(200));
  for (int i = 0; i < 1; i++) {
    std::thread tthread([&]() {
      thread_local uint64_t prev_time = 0;
      thread_local uint64_t prev_cnt = 0;
      thread_local uint64_t cnt = 0;

      struct timespec prev_ts;
      struct timespec ts;
      while (1) {
        for (int i = 0; i < 10000; i++) {
          if (cached_clock_gettime(CLOCK_REALTIME, &ts) != 0) {
            return 0;
          }

          uint64_t curr_time =
              uint64_t(ts.tv_sec) * 1000 + uint64_t(ts.tv_nsec) / 1000000;

          if (curr_time < prev_time) {
            printf(
                "[prev_cnt :%lld cnt: %lld ] [prev_time %lld curr_time    "
                "%lld] "
                "[prev_ts: %10ld-%10ld      ts: %10ld-%10ld] !!!! \r\n",
                prev_cnt, cnt, prev_time, curr_time, prev_ts.tv_sec,
                prev_ts.tv_nsec, ts.tv_sec, ts.tv_nsec);
          } else {
            // printf(
            //     "[prev_cnt :%lld cnt: %lld ] [prev_time %lld curr_time    "
            //     "%lld] "
            //     "[prev_ts: %10ld-%10ld      ts: %10ld-%10ld] \r\n",
            //     prev_cnt, cnt, prev_time, curr_time, prev_ts.tv_sec,
            //     prev_ts.tv_nsec, ts.tv_sec, ts.tv_nsec);
          };
          prev_cnt = cnt;
          prev_time = curr_time;
          prev_ts = ts;

          cnt++;
          if (cnt % 1000000000 == 0) {
            printf("cnt: %lld\r\n", cnt);
          }
        }
        std::this_thread::sleep_for(std::chrono::microseconds(1));
      }
    });
    tthread.detach();
  }

  while (1) {
    // for (int j = 0; j < 10000; j++) {
    //   struct timespec ts;
    //   if (cached_clock_gettime(CLOCK_MONOTONIC, &ts) != 0) {
    //     return 0;
    //   }
    //   int64_t curr_time = ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
    //   if (curr_time < prev_time) {
    //     printf("prev_time : %lld  curr_time: %lld \r\n", curr_time,
    //     prev_time);
    //   }
    //   prev_time = curr_time;
    // }
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
  return 0;
}
