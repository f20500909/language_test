#include <assert.h>
#include <fcntl.h>
#include <sched.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

// 日志
int g_debug_show_more_log = 0;

int g_debug_time_cached_time = 1;
int g_debug_time_cached_clock_gettime = 1;
int g_debug_time_cached_gettimeofday = 1;
uint64_t g_debug_print_frequfency = 1e9;

int g_max_ticks_multi = 150;

//一毫秒 = 1 * 1000 * 1000;
int g_cached_nsec_tolerance = 1 * 600 * 1000;
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

// read_cpu_frequency() is modified from source code of glibc.
int64_t read_cpu_frequency(bool *invariant_tsc) {
  /* We read the information from the /proc filesystem.  It contains at
     least one line like
     cpu MHz         : 497.840237
     or also
     cpu MHz         : 497.841
     We search for this line and convert the number in an integer.  */

  const int fd = open("/proc/cpuinfo", O_RDONLY);
  if (fd < 0) {
    return 0;
  }

  int64_t result = 0;
  char buf[4096];  // should be enough
  const ssize_t n = read(fd, buf, sizeof(buf));
  if (n > 0) {
    char *mhz = static_cast<char *>(memmem(buf, n, "cpu MHz", 7));

    if (mhz != NULL) {
      char *endp = buf + n;
      int seen_decpoint = 0;
      int ndigits = 0;

      /* Search for the beginning of the string.  */
      while (mhz < endp && (*mhz < '0' || *mhz > '9') && *mhz != '\n') {
        ++mhz;
      }
      while (mhz < endp && *mhz != '\n') {
        if (*mhz >= '0' && *mhz <= '9') {
          result *= 10;
          result += *mhz - '0';
          if (seen_decpoint) ++ndigits;
        } else if (*mhz == '.') {
          seen_decpoint = 1;
        }
        ++mhz;
      }

      /* Compensate for missing digits at the end.  */
      while (ndigits++ < 6) {
        result *= 10;
      }
    }

    if (invariant_tsc) {
      char *flags_pos = static_cast<char *>(memmem(buf, n, "flags", 5));
      *invariant_tsc =
          (flags_pos &&
           memmem(flags_pos, buf + n - flags_pos, "constant_tsc", 12) &&
           memmem(flags_pos, buf + n - flags_pos, "nonstop_tsc", 11));
    }
  }
  close(fd);
  return result;
}

// Returns CPU clock in mhz
// Return value must be >= 0.
int64_t read_invariant_cpu_frequency() {
  bool invariant_tsc = false;
  const int64_t freq = read_cpu_frequency(&invariant_tsc);
  if (!invariant_tsc || freq < 0) {
    assert(false);
    return 0;
  }
  return freq / uint64_t(1e6);
}

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
static uint64_t get_uint64_time() {
  static __thread uint64_t cur_time = 0;
  static __thread uint64_t walltick = 0;
  static __thread uint64_t max_ticks = 2000;
  static __thread uint64_t cpuspeed_mhz = read_invariant_cpu_frequency();
  static __thread uint64_t tick = 0;
  static __thread struct timespec tp;

  // If we are on a different cpu with unsynchronized tsc,
  // RDTSC() may be smaller than walltick
  // in this case tick will be a negative number,
  // whose unsigned value is much larger than max_ticks
  if (cur_time == 0 || (tick = RDTSC() - walltick) > max_ticks) {
    if (tick == 0) {
      max_ticks = cpuspeed_mhz * g_max_ticks_multi;
    }
    clock_gettime(CLOCK_REALTIME, &tp);
    cur_time = transform_time_to_uint64(tp);
    walltick = RDTSC();
  }

  // printf("max_ticks %lld ,walltick %lld \r\n", max_ticks, walltick);

  return cur_time;
}

unsigned int cached_time(time_t *__timer) {
#ifdef __i386__
  if (g_debug_time_cached_time) {
    if (!__timer) return 0;
    transform_uint64_to_time(get_uint64_time(), *__timer);
    return *__timer;
  }
#endif
  return ((unsigned int)::time(__timer));
};

int cached_gettimeofday(timeval *tv) {
#ifdef __i386__
  if (g_debug_time_cached_gettimeofday) {
    transform_uint64_to_time(get_uint64_time(), *tv);
    return 0;
  }
#endif
  return ::gettimeofday(tv, NULL);
}

int cached_clock_gettime(clockid_t __clock_id, struct timespec *tp) {
// TODO implement __clock_id
#ifdef __i386__
  if (g_debug_time_cached_clock_gettime) {
    if (CLOCK_REALTIME != __clock_id) {
      return ::clock_gettime(__clock_id, tp);
    } else {
      transform_uint64_to_time(get_uint64_time(), *tp);
      return 0;
    }
  }
#endif
  return ::clock_gettime(__clock_id, tp);
}

// ========================================================================
// ========================================================================
// ========================================================================
// ========= test code =========
#include <assert.h>
#include <errno.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <sys/sysinfo.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#include <algorithm>
#include <chrono>
#include <mutex>
#include <string>
#include <thread>

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

void showMaxDeviation(uint64_t sys_diff_time) {
  uint64_t prev_g_max_deviation = g_max_deviation;
  g_max_deviation = std::max((uint64_t)abs(sys_diff_time), g_max_deviation);
  if (prev_g_max_deviation != g_max_deviation) {
    printf("g_max_deviation: %lld us\r\n", g_max_deviation / 1000);
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
    cur_time = cached_time(&timer);
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

    if (cached_gettimeofday(&tv) != 0) {
      printf("!!!!!!!!!! cached_gettimeofday error!\r\n");
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
  int speed = -1;

  while (speed < 100) {
    speed = getcpuspeed_mhz(50 * 1000);
  }
  printf("getcpuspeed..\r\n");

  return speed;
}

void compare_time_diff(const std::string &type) {
  printf(
      "=================================compare_time_diff start "
      "[%s]=================================\r\n",
      type.c_str());
  uint64_t loops = 10 * 1000 * 1000;
  int i;
  struct timeval t1, t2, t3;
  int spd = read_invariant_cpu_frequency();

  uint64_t max = 0, diff = 0, nr_diff10 = 0, nr_diff100 = 0, nr_diff500 = 0;
  uint64_t tsc = RDTSC();
  int hastscp = 0;
  uint32_t aux = 0;

  cpu_set_t set;
  CPU_ZERO(&set);
  CPU_SET(0, &set);

  if (sched_setaffinity(0, sizeof(set), &set)) {
    printf("failed to set affinity\n");
    assert(false);
  }

  if (type == "custom gettimeofday") {
    printf("tsc=%llu, aux=0x%x, cpu speed %d mhz\n", tsc, aux, spd);
    // printf("getcpuspeed_mhz  10 us:%d\n", getcpuspeed_mhz(10 * 1000));
    // printf("getcpuspeed_mhz 100 us:%d\n", getcpuspeed_mhz(100 * 1000));
    printf("getcpuspeed_mhz 500 us:%d\n", getcpuspeed_mhz(500 * 1000));
  }

  for (i = 0; i < loops; ++i) {
    if (type == "system gettimeofday") {
      ::gettimeofday(&t1, NULL);
    } else if (type == "custom gettimeofday") {
      cached_gettimeofday(&t1);
    }

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
    if (t3.tv_usec > 10) ++nr_diff10;
    if (t3.tv_usec > 100) ++nr_diff100;
    if (t3.tv_usec > 500) ++nr_diff500;

    diff += t3.tv_usec;
    // if (i % 20 == 0) usleep((i%10)<<5);
  }
  printf(
      "[total cnt=%llu][max diff=%llu us] [ave diff=%llu us] [10us diff "
      "count=%llu ] "
      "[100us diff count=%llu ] [500us diff count=%llu ]\n",
      loops, max, diff / i, nr_diff10, nr_diff100, nr_diff500);
  printf(
      "=================================compare_time_diff end "
      "[%s]=================================\r\n\r\n",
      type.c_str());
}

void check_cpu_frequency() {
  printf("getcpuspeed: %d ,read_invariant_cpu_frequency: %llu  \r\n",
         getcpuspeed(), read_invariant_cpu_frequency());
}

void do_gettime_diff_check() {
  while (true) {
    struct timeval tv;
    // cached_gettimeofday(&tv);
    ::gettimeofday(&tv, NULL);
    // std::this_thread::sleep_for(std::chrono::microseconds(1));
  }
}

int main() {
  // =================测时间是否满足要求

  for (int i = 0; i < 3; i++) {
    compare_time_diff("system gettimeofday");
    compare_time_diff("custom gettimeofday");
  }

  // =================测接口
  auto func_list = {test_cached_clock_gettime};
  // auto func_list = {test_cached_gettimeofday};
  // auto func_list = {test_cached_time, test_cached_gettimeofday};
  // auto func_list = {test_cached_time, test_cached_gettimeofday,
  // test_cached_clock_gettime};

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
  int thread_per_func = 1;

  // std::this_thread::sleep_for(std::chrono::seconds(uint64_t(1e9)));

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
