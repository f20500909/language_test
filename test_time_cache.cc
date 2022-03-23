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

int gDebugDoFixBug = 0;

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
  //   printf("sizeof(__time_t): %d,sizeof(__time_t):%d\r\n", sizeof(__time_t),
  //          sizeof(__time_t));
}

// use cache to reduce system call in i386
class CTimeThread {
 public:
  CTimeThread();
  ~CTimeThread();

  void run();

  time_t cached_time(time_t *__timer);
  int cached_gettimeofday(timeval *tv);
  int cached_clock_gettime(clockid_t clock_id, struct timespec *tp);

 private:
  void do_update_time();

  uint64_t transform_time_to_integer(timeval tv);

  uint64_t transform_time_to_integer(timespec tp);

  struct timeval _tv;

  timespec _tp_monotonic;

  timespec _tp_realtime;

  uint32_t _interval__nsec;

  pthread_t _thread;
};

CTimeThread::CTimeThread() : _interval__nsec(10) {
  this->do_update_time();

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

void CTimeThread::do_update_time() {
  syscall(__NR_gettimeofday, &_tv, NULL);

  // TODO merge two clock_gettime to one syscall
  syscall(__NR_clock_gettime, CLOCK_MONOTONIC, &_tp_monotonic);
  syscall(__NR_clock_gettime, CLOCK_REALTIME, &_tp_realtime);
}

void CTimeThread::run() {
  while (true) {
    this->do_update_time();
    struct timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = _interval__nsec;
    assert(_interval__nsec < 1e6);
    while ((-1 == nanosleep(&ts, &ts)))
      ;
  }
}

time_t CTimeThread::cached_time(time_t *__timer) {
  if (!__timer) {
    return _tv.tv_sec;
  }
  *__timer = _tv.tv_sec;
  return _tv.tv_sec;
}

uint64_t CTimeThread::transform_time_to_integer(timeval tv) {
  return uint64_t(tv.tv_sec) * 1e9 + uint64_t(tv.tv_usec) * 1e3;
}

uint64_t CTimeThread::transform_time_to_integer(timespec tp) {
  return uint64_t(tp.tv_sec) * 1e9 + uint64_t(tp.tv_nsec);
}

int CTimeThread::cached_gettimeofday(timeval *tv) {
  if (!gDebugDoFixBug) {
    if (tv) *tv = _tv;
    return 0;
  }

  uint64_t prev_time = 0;
  uint64_t cur_time = 0;

  while (true) {
    prev_time = uint64_t(tv->tv_sec) * 1e9 + uint64_t(tv->tv_usec) * 1e3;
    // ========= Critical section start =========
    __sync_bool_compare_and_swap(&(tv->tv_sec), tv->tv_sec, _tv.tv_sec);
    __sync_bool_compare_and_swap(&(tv->tv_usec), tv->tv_usec, _tv.tv_usec);
    // ========= Critical section end =========
    cur_time = uint64_t(tv->tv_sec) * 1e9 + uint64_t(tv->tv_usec) * 1e3;
    if (cur_time > prev_time) {
      break;
    }
  }

  return 0;
}

int CTimeThread::cached_clock_gettime(clockid_t clock_id, struct timespec *tp) {
  if (!tp) return -1;

  if (!gDebugDoFixBug) {
    if (CLOCK_MONOTONIC == clock_id) {
      *tp = _tp_monotonic;
      return 0;
    } else if (CLOCK_REALTIME == clock_id) {
      *tp = _tp_realtime;
      return 0;
    }

    return 0;
  }

  uint64_t prev_time = 0;
  uint64_t cur_time = 0;

  while (true) {
    // prev_time = transform_time_to_integer(*tp);
    // ========= Critical section start =========
    if (CLOCK_MONOTONIC == clock_id) {
      __sync_bool_compare_and_swap(&(tp->tv_sec), tp->tv_sec,
                                   _tp_realtime.tv_sec);
      __sync_bool_compare_and_swap(&(tp->tv_nsec), tp->tv_nsec,
                                   _tp_realtime.tv_nsec);

    } else if (CLOCK_REALTIME == clock_id) {
      __sync_bool_compare_and_swap(&(tp->tv_sec), tp->tv_sec,
                                   _tp_realtime.tv_sec);
      __sync_bool_compare_and_swap(&(tp->tv_nsec), tp->tv_nsec,
                                   _tp_realtime.tv_nsec);
    }
    // ========= Critical section end =========
    // cur_time = transform_time_to_integer(*tp);
    if (cur_time >= prev_time) {
      break;
      return 0;
    }
  }

  return ::clock_gettime(clock_id, tp);
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

int cached_gettimeofday(struct timeval *__tv) {
  assert(gCTimeThread);
  if (gDebugTimeCached_gettimeofday) {
    return gCTimeThread->cached_gettimeofday(__tv);
  } else {
    return ::gettimeofday(__tv, NULL);
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

void test_cached_time() {
  thread_local uint64_t prev_time = 0;
  thread_local uint64_t prev_cnt = 0;
  thread_local uint64_t cnt = 0;

  time_t timer;

  time_t cur_time = 0;
  time_t perv_time = 0;

  while (1) {
    for (int i = 0; i < 10000; i++) {
      if (cached_time(&timer) != 0) {
        printf("!!!!!!!!!! cached_time error!\r\n");
        return;
      }

      if (cur_time < prev_time) {
        printf(
            "[test_cached_time][prev_cnt :%lld cnt: %lld ] [prev_time %lld "
            "cur_time    "
            "%lld] "
            " !!!! \r\n",
            prev_cnt, cnt, prev_time, cur_time);
      } else {
        // printf(
        //     "[test_cached_time][prev_cnt :%lld cnt: %lld ] [prev_time %lld "
        //     "cur_time    "
        //     "%lld] "
        //     " \r\n",
        //     prev_cnt, cnt, prev_time, cur_time);
      };
      prev_cnt = cnt;
      prev_time = cur_time;

      cnt++;
      if (cnt & (1 << 20) == (1 << 20)) {
        printf("cnt: %lld\r\n", cnt);
      }
    }
    std::this_thread::sleep_for(std::chrono::microseconds(10));
  }
}

void test_cached_gettimeofday() {
  thread_local uint64_t prev_time = 0;
  thread_local uint64_t cur_time = 0;
  thread_local uint64_t prev_cnt = 0;
  thread_local uint64_t cnt = 0;

  thread_local struct timeval prev_ts;
  thread_local struct timeval ts;
  while (1) {
    for (int i = 0; i < 10000; i++) {
      if (cached_gettimeofday(&ts) != 0) {
        printf("!!!!!!!!!! cached_gettimeofday error!\r\n");
        return;
      }

      cur_time = uint64_t(ts.tv_sec) * 1e6 + uint64_t(ts.tv_usec);

      if (cur_time < prev_time) {
        printf(
            "[test_cached_gettimeofday][prev_cnt :%lld cnt: %lld ] [prev_time "
            "%lld cur_time   %lld] "
            "[prev_ts: %lld-%lld      ts: "
            "%lld-%lld] !!!! \r\n",
            prev_cnt, cnt, prev_time, cur_time, (uint64_t)prev_ts.tv_sec,
            (uint64_t)prev_ts.tv_usec, (uint64_t)ts.tv_sec,
            (uint64_t)ts.tv_usec);
      } else {
        // printf(
        //     "[test_cached_gettimeofday][prev_cnt :%lld cnt: %lld ] [prev_time
        //     "
        //     "%lld cur_time   %lld] "
        //     "[prev_ts: %lld-%lld      ts: "
        //     "%lld-%lld]  \r\n",
        //     prev_cnt, cnt, prev_time, cur_time, (uint64_t)prev_ts.tv_sec,
        //     (uint64_t)prev_ts.tv_usec, (uint64_t)ts.tv_sec,
        //     (uint64_t)ts.tv_usec);
      };
      prev_cnt = cnt;
      prev_time = cur_time;
      prev_ts = ts;

      cnt++;
      if (cnt & (1 << 20) == (1 << 20)) {
        printf("cnt: %lld\r\n", cnt);
      }
    }
    // std::this_thread::sleep_for(std::chrono::microseconds(10));
  }
}

void test_cached_clock_gettime() {
  thread_local uint64_t prev_time = 0;
  thread_local uint64_t cur_time = 0;
  thread_local uint64_t prev_cnt = 0;
  thread_local uint64_t cnt = 0;

  struct timespec prev_ts;
  struct timespec ts;
  while (1) {
    for (int i = 0; i < 10000; i++) {
      if (cached_clock_gettime(CLOCK_REALTIME, &ts) != 0) {
        printf("!!!!!!!!!! cached_clock_gettime error!\r\n");
        return;
      }

      cur_time = uint64_t(ts.tv_sec) * 1e9 + uint64_t(ts.tv_nsec);

      if (cur_time < prev_time) {
        printf(
            "[test_cached_clock_gettime][prev_cnt :%lld cnt: %lld ] [prev_time "
            "%lld cur_time    "
            "%lld] "
            "[prev_ts: %10ld-%10ld      ts: %10ld-%10ld] !!!! \r\n",
            prev_cnt, cnt, prev_time, cur_time, (uint64_t)prev_ts.tv_sec,
            (uint64_t)prev_ts.tv_nsec, (uint64_t)ts.tv_sec,
            (uint64_t)ts.tv_nsec);
      } else {
        // printf(
        //     "[test_cached_clock_gettime][prev_cnt :%lld cnt: %lld ]"
        //     "[prev_time "
        //     "%lld cur_time    "
        //     "%lld] "
        //     "[prev_ts: %10ld-%10ld      ts: %10ld-%10ld] \r\n",
        //     prev_cnt, cnt, prev_time, cur_time, (uint64_t)prev_ts.tv_sec,
        //     (uint64_t)prev_ts.tv_nsec, (uint64_t)ts.tv_sec,
        //     (uint64_t)ts.tv_nsec);
      };
      prev_cnt = cnt;
      prev_time = cur_time;
      prev_ts = ts;

      cnt++;
      if (cnt & (1 << 20) == (1 << 20)) {
        printf("cnt: %lld\r\n", cnt);
      }
    }
    std::this_thread::sleep_for(std::chrono::microseconds(10));
  }
}

int main() {
//   showPlatform();

//   gCTimeThread = new CTimeThread();
//   std::this_thread::sleep_for(std::chrono::milliseconds(200));
//   //   for (int i = 0; i < 1; i++) {
//   //     std::thread tthread(test_cached_time);
//   //     tthread.detach();
//   //   }

//   for (int i = 0; i < 1; i++) {
//     std::thread tthread(test_cached_gettimeofday);
//     tthread.detach();
//   }

//   //   for (int i = 0; i < 1; i++) {
//   //     std::thread tthread(test_cached_clock_gettime);
//   //     tthread.detach();
//   //   }

//   while (1) {
//     std::this_thread::sleep_for(std::chrono::milliseconds(1));
//   }
  return 0;
}
