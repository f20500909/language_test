#include <assert.h>
#include <errno.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

// 日志
int gDebugShowMoreLog = 0;

int gDebugTimeCached_time = 1;
int gDebugTimeCached_clock_gettime = 1;
int gDebugTimeCached_gettimeofday = 1;
uint64_t gDebugPrintFrequfency = 1e9;

//一毫秒 = 1 * 1000 * 1000;

int gCachedNsecTolerance = 100000 * 1000;
int g_test_cached_time_sleep_nsec = 100 * 1000;

template <typename TYPE, void (TYPE::*Run)()>
void *thread_rounter(void *param) {
  TYPE *p = (TYPE *)param;
  p->run();
  return NULL;
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

  pthread_t _thread;
  pthread_rwlock_t _tv_rwlock;
  pthread_rwlock_t _tp_monotonic_rwlock;
  pthread_rwlock_t _tp_realtime_rwlock;

  struct timeval _tv;
  struct timezone _tz;
  timespec _tp_monotonic;
  timespec _tp_realtime;

  uint64_t _update_time_cnt = 0;
  uint64_t _sleep_interval = uint64_t(1 * 1e1);
};

CTimeThread::CTimeThread() {
  this->do_update_time();

  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

  pthread_rwlock_init(&_tv_rwlock, NULL);            //初始化读写锁
  pthread_rwlock_init(&_tp_realtime_rwlock, NULL);   //初始化读写锁
  pthread_rwlock_init(&_tp_monotonic_rwlock, NULL);  //初始化读写锁

  int errcode = pthread_create(&_thread, &attr,
                               thread_rounter<CTimeThread, &CTimeThread::run>,
                               (void *)this);
  if (errcode != 0) {
    printf("!!!!!!!!!! pthread_create error!");
  }
}

CTimeThread::~CTimeThread() {}

void CTimeThread::do_update_time() {
  pthread_rwlock_wrlock(&_tv_rwlock);  // write lock
  ::gettimeofday(&_tv, NULL);
  pthread_rwlock_unlock(&_tv_rwlock);  // unlock write lock

  pthread_rwlock_wrlock(&_tp_realtime_rwlock);  // write lock
  ::clock_gettime(CLOCK_REALTIME, &_tp_realtime);
  pthread_rwlock_unlock(&_tp_realtime_rwlock);  // unlock write lock

  pthread_rwlock_wrlock(&_tp_monotonic_rwlock);  // write lock
  ::clock_gettime(CLOCK_MONOTONIC, &_tp_monotonic);
  pthread_rwlock_unlock(&_tp_monotonic_rwlock);  // unlock write lock
}

void CTimeThread::run() {
  // do_sched_setaffinity(0);
  while (true) {
    this->do_update_time();

    if ((_update_time_cnt % _sleep_interval) == 0) {
      // printf("_update_time_cnt %d\r\n",_update_time_cnt);
      struct timespec ts;
      ts.tv_sec = 0;
      ts.tv_nsec = g_test_cached_time_sleep_nsec;  // 100 us
      nanosleep(&ts, &ts);
    }
    _update_time_cnt++;
  }
}

time_t CTimeThread::cached_time(time_t *__timer) {
  time_t t = 0;
  pthread_rwlock_rdlock(&_tv_rwlock);  // read lock
  t = static_cast<time_t>(_tv.tv_sec);
  pthread_rwlock_unlock(&_tv_rwlock);  // unlock read lock

  if (__timer) *__timer = t;
  return t;
}

int CTimeThread::cached_gettimeofday(timeval *tv) {
  assert(tv);
  if (!tv) return -1;  // failure

  pthread_rwlock_rdlock(&_tv_rwlock);  // read lock
  *tv = _tv;
  pthread_rwlock_unlock(&_tv_rwlock);  // unlock read lock

  return 0;  // success
}

int CTimeThread::cached_clock_gettime(clockid_t clock_id, struct timespec *tp) {
  assert(tp);
  if (!tp) return -1;  // failure

  if (CLOCK_REALTIME == clock_id) {
    pthread_rwlock_rdlock(&_tp_realtime_rwlock);  // read lock
    *tp = _tp_realtime;
    pthread_rwlock_unlock(&_tp_realtime_rwlock);  // unlock read lock
    return 0;                                     // success
  } else if (CLOCK_MONOTONIC == clock_id) {
    pthread_rwlock_rdlock(&_tp_monotonic_rwlock);  // read lock
    *tp = _tp_monotonic;
    pthread_rwlock_unlock(&_tp_monotonic_rwlock);  // unlock read lock
    return 0;                                      // success
  }

  return ::clock_gettime(clock_id, tp);
}

CTimeThread *gCTimeThread = new CTimeThread();

unsigned int cached_time(time_t *__timer) {
  assert(gCTimeThread);
  if (gDebugTimeCached_time) {
    return (unsigned int)(gCTimeThread->cached_time(__timer));
  }

  return ((unsigned int)::time(__timer));
};

int cached_gettimeofday(timeval *tv) {
  assert(gCTimeThread);
  if (gDebugTimeCached_gettimeofday) {
    return gCTimeThread->cached_gettimeofday(tv);
  } else {
    return ::gettimeofday(tv, NULL);
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

// ========= test code =========
#include <sys/sysinfo.h>
#include <unistd.h>

#include <chrono>
#include <mutex>
#include <thread>

uint64_t transform_time_to_uint64(const timeval &tv) {
  return uint64_t(tv.tv_sec) * uint64_t(1e9) +
         uint64_t(tv.tv_usec) * uint64_t(1e3);
}

uint64_t transform_time_to_uint64(const timespec &tp) {
  return uint64_t(tp.tv_sec) * uint64_t(1e9) + uint64_t(tp.tv_nsec);
}

void transform_uint64_to_time(uint64_t time_uint64, timespec &tp) {
  tp.tv_sec = time_uint64 / uint64_t(1e9);
  tp.tv_nsec = time_uint64 % uint64_t(1e9);
}

void transform_uint64_to_time(uint64_t time_uint64, timeval &tv) {
  tv.tv_sec = time_uint64 / uint64_t(1e9);
  tv.tv_usec = (time_uint64 % uint64_t(1e9)) / (uint64_t(1e3));
  return;
}

void transform_uint64_to_time_t(uint64_t time_uint64, time_t &t) {
  t = time_uint64 / uint64_t(1e9);
  return;
}

void do_sched_setaffinity(int cpu_id) {
  cpu_set_t mask;
  CPU_ZERO(&mask);

  CPU_SET(cpu_id, &mask);

  int tid = syscall(SYS_gettid);

  if (sched_setaffinity(tid, sizeof(cpu_set_t), &mask) == -1) {
    printf("warning: could not set CPU affinity, continuing...\n");
    assert(false);
    return;
  }
  struct timespec ts;
  ts.tv_sec = 0;
  ts.tv_nsec = 100000000;

  while ((-1 == nanosleep(&ts, &ts)))
    ;
  // printf("   ====tid: %d cpu_id: %d\r\n", tid, cpu_id);

  return;
}
void showPlatform() {
#ifdef __x86_64__
  printf("__x86_64__\r\n");
#elif __i386__
  printf("__i386__\r\n");
#endif
  //   printf("sizeof(__time_t): %d,sizeof(__time_t):%d\r\n", sizeof(__time_t),
  //          sizeof(__time_t));

  printf("UINT64_MAX : %llu\r\n", UINT64_MAX);
}

void test_cached_time(int cpu_id) {
  do_sched_setaffinity(cpu_id);
  thread_local uint64_t prev_time = 0;
  thread_local uint64_t cur_time = 0;
  thread_local uint64_t sys_cur_time = 0;
  thread_local uint64_t prev_cnt = 0;
  thread_local uint64_t cnt = 0;
  thread_local int64_t sys_diff_time = 0;

  time_t timer;

  while (1) {
    cur_time = cached_time(&timer);
    sys_cur_time = ::time(&timer);

    sys_diff_time = cur_time - sys_cur_time;

    if (cur_time < prev_time || (abs(sys_diff_time) >= gCachedNsecTolerance)) {
      printf(
          "[test_cached_time][prev_cnt :%llu cnt: %llu ]"
          "[prev_time %llu cur_time    %llu sys_diff_time %lld] "
          " !!!! \r\n",
          prev_cnt, cnt, prev_time, cur_time, sys_diff_time);
    } else {
      if (gDebugShowMoreLog)
        printf(
            "[test_cached_time][prev_cnt :%llu cnt: %llu ]"
            "[prev_time %llu cur_time    %llu sys_diff_time %lld] "
            " \r\n",
            prev_cnt, cnt, prev_time, cur_time, sys_diff_time);
    };
    prev_cnt = cnt;
    prev_time = cur_time;

    cnt++;
    if ((cnt % gDebugPrintFrequfency) == 0) {
      printf("[test_cached_time][cpu_id:%d] cnt: %llu\r\n", cpu_id, cnt);
    }
  }
}

void test_cached_gettimeofday(int cpu_id) {
  do_sched_setaffinity(cpu_id);
  thread_local uint64_t prev_time = 0;
  thread_local uint64_t cur_time = 0;
  thread_local uint64_t sys_cur_time = 0;
  thread_local uint64_t prev_cnt = 0;
  thread_local uint64_t cnt = 0;
  thread_local int64_t sys_diff_time = 0;

  thread_local struct timeval prev_ts;
  thread_local struct timeval tv;
  while (1) {
    if (::gettimeofday(&tv, NULL) != 0) {
      printf("!!!!!!!!!! gettimeofday error!\r\n");
      return;
    }
    sys_cur_time = transform_time_to_uint64(tv);

    if (cached_gettimeofday(&tv) != 0) {
      printf("!!!!!!!!!! cached_gettimeofday error!\r\n");
      return;
    }
    cur_time = transform_time_to_uint64(tv);

    sys_diff_time = cur_time - sys_cur_time;

    if (cur_time < prev_time || (abs(sys_diff_time) >= gCachedNsecTolerance)) {
      printf(
          "[test_cached_gettimeofday][prev_cnt :%llu cnt: %llu ] "
          "[prev_time %llu cur_time  %llu sys_diff_time  %lld]] "
          "[prev_ts: %llu-%llu      tv: "
          "%llu-%llu] !!!! \r\n",
          prev_cnt, cnt, prev_time, cur_time, sys_diff_time,
          (uint64_t)prev_ts.tv_sec, (uint64_t)prev_ts.tv_usec,
          (uint64_t)tv.tv_sec, (uint64_t)tv.tv_usec);
    } else {
      if (gDebugShowMoreLog)
        printf(
            "[test_cached_gettimeofday][prev_cnt :%llu cnt: %llu ] "
            "[prev_time %llu cur_time  %llu sys_diff_time  %lld]] "
            "[prev_ts: %llu-%llu      tv: "
            "%llu-%llu] \r\n",
            prev_cnt, cnt, prev_time, cur_time, sys_diff_time,
            (uint64_t)prev_ts.tv_sec, (uint64_t)prev_ts.tv_usec,
            (uint64_t)tv.tv_sec, (uint64_t)tv.tv_usec);
    };
    prev_cnt = cnt;
    prev_time = cur_time;
    prev_ts = tv;

    cnt++;
    if ((cnt % gDebugPrintFrequfency) == 0) {
      printf("[test_cached_gettimeofday][cpu_id:%d] cnt: %llu\r\n", cpu_id,
             cnt);
    }
    // std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
}

void test_cached_clock_gettime(int cpu_id) {
  do_sched_setaffinity(cpu_id);
  thread_local uint64_t prev_time = 0;
  thread_local uint64_t sys_cur_time = 0;
  thread_local uint64_t cur_time = 0;
  thread_local uint64_t prev_cnt = 0;
  thread_local uint64_t cnt = 0;
  thread_local int64_t sys_diff_time = 0;

  struct timespec prev_ts;
  struct timespec ts;
  while (1) {
    if (::clock_gettime(CLOCK_REALTIME, &ts) != 0) {
      printf("!!!!!!!!!! cached_clock_gettime error!\r\n");
      return;
    }
    sys_cur_time = transform_time_to_uint64(ts);

    if (cached_clock_gettime(CLOCK_REALTIME, &ts) != 0) {
      printf("!!!!!!!!!! cached_clock_gettime error!\r\n");
      return;
    }
    cur_time = transform_time_to_uint64(ts);

    sys_diff_time = cur_time - sys_cur_time;

    if (cur_time < prev_time || (abs(sys_diff_time) >= gCachedNsecTolerance)) {
      printf(
          "[test_cached_clock_gettime][prev_cnt :%llu cnt: %llu ]"
          " [prev_time %llu cur_time    %llu sys_diff_time    %lld] "
          "[prev_ts: %llu-%llu      ts: %llu-%llu] !!!! \r\n",
          prev_cnt, cnt, prev_time, cur_time, sys_diff_time,
          (uint64_t)prev_ts.tv_sec, (uint64_t)prev_ts.tv_nsec,
          (uint64_t)ts.tv_sec, (uint64_t)ts.tv_nsec);
    } else {
      if (gDebugShowMoreLog)
        printf(
            "[test_cached_clock_gettime][prev_cnt :%llu cnt: %llu ]"
            " [prev_time %llu cur_time    %llu sys_diff_time    %lld] "
            "[prev_ts: %llu-%llu      ts: %llu-%llu] \r\n",
            prev_cnt, cnt, prev_time, cur_time, sys_diff_time,
            (uint64_t)prev_ts.tv_sec, (uint64_t)prev_ts.tv_nsec,
            (uint64_t)ts.tv_sec, (uint64_t)ts.tv_nsec);
    };
    prev_cnt = cnt;
    prev_time = cur_time;
    prev_ts = ts;

    cnt++;
    if ((cnt % gDebugPrintFrequfency) == 0) {
      printf("[test_cached_clock_gettime][cpu_id:%d] cnt: %llu\r\n", cpu_id,
             cnt);
    }
  }
}

void compare_time_diff(const std::string &type) {
  printf(
      "=================================compare_time_diff start "
      "[%s]=================================\r\n",
      type.c_str());
  uint64_t loops = 1 * 1000;
  int i;
  struct timeval t1, t2, t3;

  uint64_t max = 0, diff = 0, nr_diff_p1 = 0, nr_diff_p2 = 0, nr_diff_p3 = 0;

  int hastscp = 0;

  cpu_set_t set;
  CPU_ZERO(&set);
  CPU_SET(0, &set);

  // if (sched_setaffinity(1, sizeof(set), &set)) {
  //   printf("failed to set affinity\n");
  //   assert(false);
  // }

  for (i = 0; i < loops; ++i) {
    if (type == "system gettimeofday") {
      ::gettimeofday(&t1, NULL);
    } else if (type == "custom gettimeofday") {
      cached_gettimeofday(&t1);
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1));  // 主动让出cpu

    ::gettimeofday(&t2, NULL);

    if (timercmp(&t1, &t2, >)) {
      timersub(&t1, &t2, &t3);
    } else {
      timersub(&t2, &t1, &t3);
    }

    // printf("t1=%u.%06u\t", t1.tv_sec, t1.tv_usec);
    // printf("t2=%u.%06u\t", t2.tv_sec, t2.tv_usec);
    // printf("diff=%u.%06u\n", t3.tv_sec, t3.tv_usec);

    if (max < t3.tv_usec) max = t3.tv_usec;
    if (t3.tv_usec > 12 * 100) ++nr_diff_p1;
    if (t3.tv_usec > 15 * 100) ++nr_diff_p2;
    if (t3.tv_usec > 20 * 100) ++nr_diff_p3;

    diff += t3.tv_usec;
    // if (i % 20 == 0) usleep((i%10)<<5);
  }
  printf(
      "[total cnt=%llu][max diff=%llu us] [ave diff=%llu us] [1.2ms diff "
      "count=%llu ] "
      "[1.5ms diff count=%llu ] [2.0ms diff count=%llu ]\n",
      loops, max, diff / loops, nr_diff_p1, nr_diff_p2, nr_diff_p3);
  printf(
      "=================================compare_time_diff end "
      "[%s]=================================\r\n\r\n",
      type.c_str());
}

int main() {
  showPlatform();
  // =================测时间是否满足要求

  // for (int i = 0; i < 100; i++) {
  //   compare_time_diff("system gettimeofday");
  //   compare_time_diff("custom gettimeofday");
  // }

  // auto func_list = {test_cached_time};
  // auto func_list = {test_cached_gettimeofday};
  auto func_list = {test_cached_time, test_cached_gettimeofday,
                    test_cached_clock_gettime};

  int core_number = get_nprocs();

  //  e.g. :core number is 8
  //  0 1 2 3 4 5 6 7
  //  thread 0   : cached time
  //  thread 1   : empty
  //  thread 2-3 : test_cached_time
  //  thread 4-5 : test_cached_gettimeofday
  //  thread 6-7 : test_cached_clock_gettime

  // std::this_thread::sleep_for(std::chrono::seconds(uint64_t(1e9)));
  int cpu_id = 1;
  // int thread_per_func = (core_number - 1) / func_list.size();
  int thread_per_func = 1;

  for (auto &func : func_list) {
    printf("== func :%d", func);
    for (int thread_num = 0; thread_num < thread_per_func; thread_num++) {
      cpu_id++;
      std::thread tthread(func, cpu_id);
      tthread.detach();
      printf("  cpu_id:%d  ", cpu_id);

      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    printf("==\r\n");
  }

  std::this_thread::sleep_for(std::chrono::seconds(uint64_t(1e9)));
  return 0;
}
