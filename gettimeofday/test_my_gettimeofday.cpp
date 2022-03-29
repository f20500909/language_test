
#include <sched.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

// 日志
int g_debug_show_more_log = 0;

int g_debug_time_cached_time = 1;
int g_debug_time_cached_clock_gettime = 1;
int g_debug_time_cached_gettimeofday = 1;
uint64_t g_debug_print_frequfency = 1e9;

//一毫秒 = 1 * 1000 * 1000;

int g_cached_nsec_tolerance = 2 * 100 * 1000;
uint64_t g_max_deviation = 0;

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

void transform_uint64_to_time(uint64_t time_uint64, clockid_t __clock_id,
                              timespec tp) {
  tp.tv_sec = time_uint64 / uint64_t(1e9);
  tp.tv_nsec = time_uint64 % uint64_t(1e9);
}

void transform_uint64_to_time(uint64_t time_uint64, timeval &tv) {
  tv.tv_sec = time_uint64 / uint64_t(1e9);
  tv.tv_usec = (time_uint64 % uint64_t(1e9)) / (uint64_t(1e3));
  return;
}

void transform_uint64_to_time(uint64_t time_uint64, time_t &t) {
  t = time_uint64 / uint64_t(1e9);
  return;
}

/*
 * Checked against the Intel manual and GCC --hpreg
 *
 * volatile because the tsc always changes without the compiler knowing it.
 */

#ifdef __x86_64__
#define RDTSC()                                       \
  ({                                                  \
    register uint32_t a, d;                           \
    __asm__ __volatile__("rdtsc" : "=a"(a), "=d"(d)); \
    (((uint64_t)a) + (((uint64_t)d) << 32));          \
  })
#else
#define RDTSC()                                \
  ({                                           \
    register uint64_t tim;                     \
    __asm__ __volatile__("rdtsc" : "=A"(tim)); \
    tim;                                       \
  })
#endif

#define RDTSCP(aux)                                        \
  ({                                                       \
    register uint64_t tim;                                 \
    __asm__ __volatile__("rdtscp" : "=A"(tim), "=c"(aux)); \
    tim;                                                   \
  })

// Static variables are initialized to 0
static int has_rdtscp;
static int cpuid;

static inline int test_rdtscp() {
  register uint32_t edx;
  __asm__ __volatile__(
      "movl $0x80000001, %%eax \n\t"
      "cpuid"
      : "=d"(edx)
      :
      : "eax", "ebx", "ecx");
  return (edx & (1U << 27));
}

/*
 * Returns CPU clock in mhz
 * Notice that the function will cost the calling thread to sleep wait_us us
 */
static inline int getcpuspeed_mhz(unsigned int wait_us) {
  uint64_t tsc1, tsc2;
  struct timespec t;

  t.tv_sec = 0;
  t.tv_nsec = wait_us * 1000;

  tsc1 = RDTSC();

  // If sleep failed, result is unexpected
  if (nanosleep(&t, NULL)) {
    return -1;
  }

  tsc2 = RDTSC();

  return (tsc2 - tsc1) / (wait_us);
}

static int getcpuspeed() {
  static int speed = -1;

  while (speed < 100) {
    speed = getcpuspeed_mhz(50 * 1000);
  }

  return speed;
}

#define TIME_ADD_US(a, usec)          \
  do {                                \
    (a)->tv_usec += usec;             \
    while ((a)->tv_usec >= 1000000) { \
      (a)->tv_sec++;                  \
      (a)->tv_usec -= 1000000;        \
    }                                 \
  } while (0)

// Compile with -O2 to optimize mul/div instructions
// The performance is restricted by 2 factors:
//    1, the rdtsc instruction
//    2, division
//
// Another restriction for this function:
//    The caller thread should run on one CPU or on SMP with sinchronized TSCs,
// otherwise, rdtsc instruction will differ between multiple CPUs.
//    The good thing is that most multicore CPUs are shipped with sinchronized
//    TSCs.
//
static uint64_t my_gettime_uint64() {
  static __thread uint64_t cur_time = 0;
  static __thread uint64_t walltick = 0;
  static __thread int cpuspeed_mhz = 0;
  static __thread unsigned int max_ticks = 2000;
  uint64_t tick = 0;

  // If we are on a different cpu with unsynchronized tsc,
  // RDTSC() may be smaller than walltick
  // in this case tick will be a negative number,
  // whose unsigned value is much larger than max_ticks
  if (cur_time % uint64_t(1e9) == 0 || cpuspeed_mhz == 0 ||
      (tick = RDTSC() - walltick) > max_ticks) {
    if (tick == 0 || cpuspeed_mhz == 0) {
      cpuspeed_mhz = getcpuspeed();
      max_ticks = cpuspeed_mhz;
    }
    struct timeval tv;
    gettimeofday(&tv, NULL);
    cur_time = transform_time_to_uint64(tv);
    walltick = RDTSC();
  }

  return cur_time;
}

unsigned int vdso_time(time_t *__timer) {
  if (g_debug_time_cached_time) {
    if (!__timer) return 0;

    transform_uint64_to_time(my_gettime_uint64(), *__timer);
    return *__timer;
  }

  return ((unsigned int)::time(__timer));
};

int vdso_gettimeofday(timeval *tv) {
  if (g_debug_time_cached_gettimeofday) {
    transform_uint64_to_time(my_gettime_uint64(), *tv);
    return 0;
  } else {
    return ::gettimeofday(tv, NULL);
  }
}

int vdso_clock_gettime(clockid_t __clock_id, struct timespec *tp) {
  if (g_debug_time_cached_clock_gettime) {
    if (CLOCK_REALTIME != __clock_id) {
      return ::clock_gettime(__clock_id, tp);
    } else {
      transform_uint64_to_time(my_gettime_uint64(), *tp);
      return 0;
    }
  } else {
    return ::clock_gettime(__clock_id, tp);
  }
}

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

#include <algorithm>

template <typename TYPE, void (TYPE::*Run)()>
void *thread_rounter(void *param) {
  TYPE *p = (TYPE *)param;
  p->run();
  return NULL;
}

void do_sched_setaffinity(int cpu_id) {
  cpu_set_t mask;   // cpu核的集合
  CPU_ZERO(&mask);  // 将集合置为空集

  CPU_SET(cpu_id, &mask);  // 设置亲和力值

  int tid = syscall(SYS_gettid);

  if (sched_setaffinity(tid, sizeof(cpu_set_t), &mask) ==
      -1)  // 设置线程cpu亲和力
  {
    printf("warning: could not set CPU affinity, continuing...\n");
    assert(false);
    return;
  }
  struct timespec ts;
  ts.tv_sec = 1;
  ts.tv_nsec = 100000000;

  while ((-1 == nanosleep(&ts, &ts)))
    ;
  printf("   ====tid: %d cpu_id: %d\r\n", tid, cpu_id);  // 打印这是第几个线程

  return;
}

// ========= test code =========
#include <sys/sysinfo.h>
#include <unistd.h>

#include <chrono>
#include <mutex>
#include <thread>

void showMaxDeviation(uint64_t sys_diff_time) {
  uint64_t prev_g_max_deviation = g_max_deviation;
  g_max_deviation = std::max((uint64_t)abs(sys_diff_time), g_max_deviation);
  if (prev_g_max_deviation != g_max_deviation) {
    printf("g_max_deviation: %lld\r\n", g_max_deviation);
  }
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
    cur_time = vdso_time(&timer);
    sys_cur_time = ::time(&timer);

    sys_diff_time = cur_time - sys_cur_time;

    if (cur_time < prev_time ||
        (abs(sys_diff_time) >= g_cached_nsec_tolerance)) {
      printf(
          "[test_cached_time][prev_cnt :%llu cnt: %llu ]"
          "[prev_time %llu cur_time    %llu sys_diff_time %lld] "
          " !!!! \r\n",
          prev_cnt, cnt, prev_time, cur_time, sys_diff_time);
    } else {
      if (g_debug_show_more_log)
        printf(
            "[test_cached_time][prev_cnt :%llu cnt: %llu ]"
            "[prev_time %llu cur_time    %llu sys_diff_time %lld] "
            " \r\n",
            prev_cnt, cnt, prev_time, cur_time, sys_diff_time);
    };
    prev_cnt = cnt;
    prev_time = cur_time;

    cnt++;
    if ((cnt % g_debug_print_frequfency) == 0) {
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

    if (vdso_gettimeofday(&tv) != 0) {
      printf("!!!!!!!!!! vdso_gettimeofday error!\r\n");
      return;
    }
    cur_time = transform_time_to_uint64(tv);

    sys_diff_time = cur_time - sys_cur_time;

    if (cur_time < prev_time ||
        (abs(sys_diff_time) >= g_cached_nsec_tolerance)) {
      printf(
          "[test_cached_gettimeofday][prev_cnt :%llu cnt: %llu ] "
          "[prev_time %llu cur_time  %llu sys_diff_time  %lld]] "
          "[prev_ts: %llu-%llu      tv: "
          "%llu-%llu] !!!! \r\n",
          prev_cnt, cnt, prev_time, cur_time, sys_diff_time,
          (uint64_t)prev_ts.tv_sec, (uint64_t)prev_ts.tv_usec,
          (uint64_t)tv.tv_sec, (uint64_t)tv.tv_usec);
    } else {
      if (g_debug_show_more_log)
        printf(
            "[test_cached_gettimeofday][prev_cnt :%llu cnt: %llu ] "
            "[prev_time %llu cur_time  %llu sys_diff_time  %lld]] "
            "[prev_ts: %llu-%llu      tv: "
            "%llu-%llu] \r\n",
            prev_cnt, cnt, prev_time, cur_time, sys_diff_time,
            (uint64_t)prev_ts.tv_sec, (uint64_t)prev_ts.tv_usec,
            (uint64_t)tv.tv_sec, (uint64_t)tv.tv_usec);
    };

    showMaxDeviation(sys_diff_time);
    prev_cnt = cnt;
    prev_time = cur_time;
    prev_ts = tv;

    cnt++;
    if ((cnt % g_debug_print_frequfency) == 0) {
      printf("[test_cached_gettimeofday][cpu_id:%d] cnt: %llu\r\n", cpu_id,
             cnt);
    }
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
      printf("!!!!!!!!!! vdso_clock_gettime error!\r\n");
      return;
    }
    sys_cur_time = transform_time_to_uint64(ts);

    if (vdso_clock_gettime(CLOCK_REALTIME, &ts) != 0) {
      printf("!!!!!!!!!! vdso_clock_gettime error!\r\n");
      return;
    }
    cur_time = transform_time_to_uint64(ts);

    sys_diff_time = cur_time - sys_cur_time;

    if (cur_time < prev_time ||
        (abs(sys_diff_time) >= g_cached_nsec_tolerance)) {
      printf(
          "[test_cached_clock_gettime][prev_cnt :%llu cnt: %llu ]"
          " [prev_time %llu cur_time    %llu sys_diff_time    %lld] "
          "[prev_ts: %llu-%llu      ts: %llu-%llu] !!!! \r\n",
          prev_cnt, cnt, prev_time, cur_time, sys_diff_time,
          (uint64_t)prev_ts.tv_sec, (uint64_t)prev_ts.tv_nsec,
          (uint64_t)ts.tv_sec, (uint64_t)ts.tv_nsec);
    } else {
      if (g_debug_show_more_log)
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
    if ((cnt % g_debug_print_frequfency) == 0) {
      printf("[test_cached_clock_gettime][cpu_id:%d] cnt: %llu\r\n", cpu_id,
             cnt);
    }
  }
}

void compare_time_diff() {
  printf("===========compare_time_diff start===========\r\n\r\n");
  uint64_t loops = 10 * 1000 * 1000;
  int i;
  struct timeval t1, t2, t3;
  int spd = getcpuspeed();

  uint64_t max = 0, diff = 0, nr_diff10 = 0, nr_diff100 = 0, nr_diff500 = 0;
  uint64_t tsc = RDTSC();
  int hastscp = 0;
  uint32_t aux = 0;

  cpu_set_t set;
  CPU_ZERO(&set);
  CPU_SET(0, &set);

  if ((hastscp = test_rdtscp())) {
    printf("This machine supports rdtscp.\n");
  } else {
    printf("This machine does not support rdtscp.\n");
  }

  if (hastscp) {
    printf("hastscp\n");
    tsc = RDTSCP(aux);
  } else {
    printf("no hastscp\n");
    tsc = RDTSC();
  }

  if (sched_setaffinity(0, sizeof(set), &set)) {
    printf("failed to set affinity\n");
  }

  printf("tsc=%lu, aux=0x%x, cpu speed %d mhz\n", tsc, aux, spd);
  printf("getspeed_010:%d\n", getcpuspeed_mhz(10 * 1000));
  printf("getspeed_100:%d\n", getcpuspeed_mhz(100 * 1000));
  printf("getspeed_500:%d\n", getcpuspeed_mhz(500 * 1000));

  for (i = 0; i < loops; ++i) {
#if 1
    vdso_gettimeofday(&t1);
#else
    gettimeofday(&t1, NULL);
#endif
    gettimeofday(&t2, NULL);

    if (timercmp(&t1, &t2, >)) {
      timersub(&t1, &t2, &t3);
    } else {
      timersub(&t2, &t1, &t3);
    }

    // printf("t1=%u.%06u\t", t1.tv_sec, t1.tv_usec);
    // printf("t2=%u.%06u\t", t2.tv_sec, t2.tv_usec);
    // printf("diff=%u.%06u\n", t3.tv_sec, t3.tv_usec);

    if (max < t3.tv_usec) max = t3.tv_usec;
    if (t3.tv_usec > 10) ++nr_diff10;
    if (t3.tv_usec > 100) ++nr_diff100;
    if (t3.tv_usec > 500) ++nr_diff500;

    diff += t3.tv_usec;
    // if (i % 20 == 0) usleep((i%10)<<5);
  }
  printf(
      "[total cnt=%llu][max diff=%llu] [ave diff=%llu] [10us diff count=%llu ] "
      "[100us diff count=%llu ] [500us diff count=%llu ]\n",
      loops, max, diff / i, nr_diff10, nr_diff100, nr_diff500);
  printf("===========compare_time_diff end===========\r\n\r\n");
}

int main() {
  auto func_list = {test_cached_gettimeofday};
  // auto func_list = {test_cached_time, test_cached_gettimeofday};
  // auto func_list = {test_cached_time, test_cached_gettimeofday,
  //                   test_cached_clock_gettime};

  int core_number = get_nprocs();

  //  e.g. :core number is 8
  //  0 1 2 3 4 5 6 7
  //  thread 0   : cached time
  //  thread 1   : empty
  //  thread 2-3 : test_cached_time
  //  thread 4-5 : test_cached_gettimeofday
  //  thread 6-7 : test_cached_clock_gettime

  int cpu_id = 1;
  // int thread_per_func = (core_number - 1) / func_list.size();
  int thread_per_func = 2;

  // std::this_thread::sleep_for(std::chrono::seconds(uint64_t(1e9)));

  compare_time_diff();
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
