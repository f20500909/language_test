
#include <assert.h>
#include <errno.h>
#include <memory.h>
#include <pthread.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <msr.h>

static inline int getcpuspeed_mhz(unsigned int wait_us)

{
  u_int64_t tsc1, tsc2;
  struct timespec t;

  t.tv_sec = 0;
  t.tv_nsec = wait_us * 1000;

  rdtscll(tsc1);

  // If sleep failed, result is unexpected, the caller should retry
  if (nanosleep(&t, NULL)) return -1;
  rdtscll(tsc2);
  return (tsc2 - tsc1) / (wait_us);
}

int getcpuspeed() {
  static int speed = -1;
  while (speed < 100) speed = getcpuspeed_mhz(50 * 1000);
  return speed;
}

int my_gettimeofday(struct timeval *tv) {
  u_int64_t tick = 0;

  // TSC偏移大于这个值，就需要重新获得系统时间
  static unsigned int max_ticks = 80000000;
  rdtscll(tick);
  if (walltime.tv_sec == 0 || cpuspeed_mhz == 0 ||
      (tick - walltick) > max_ticks) {
    if (tick == 0 || cpuspeed_mhz == 0) {
      cpuspeed_mhz = getcpuspeed();
      max_ticks = cpuspeed_mhz * RELOAD_TIME_US;
    }
    // printf("gettimeofday again\n");
    gettimeofday(tv, NULL);
    memcpy(&walltime, tv, sizeof(walltime));
    rdtscll(walltick);
    return 0;
  }

  memcpy(tv, &walltime, sizeof(walltime));
  // if RELOAD_TIME_US is 1, we are in the same us, no need to adjust tv
#if RELOAD_TIME_US > 1
  {
    uint32_t t;
    t = ((uint32_t)tick) / cpuspeed_mhz;
    TIME_ADD_US(tv, t);  // add 1 us
  }
#endif
  return 0;
}