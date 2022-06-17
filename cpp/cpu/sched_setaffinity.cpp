#include <errno.h>
#include <pthread.h>
#include <sched.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SCHED_SETAFFINITY 21

void update_sched_setaffinity() {}
int gSchedSetaffinityCount = 0;

int main(int argc, char **argv) {
  int cpus = 0;
  int i = 0;
  cpu_set_t mask;
  cpu_set_t get;

  cpus = sysconf(_SC_NPROCESSORS_ONLN);
  printf("cpus: %d\n", cpus);

  CPU_ZERO(&mask); /* 初始化set集，将set置为空*/
  /*将本进程绑定到CPU2上*/
  CPU_SET(1, &mask);
  CPU_SET(4, &mask);
  CPU_SET(6, &mask);
  if (sched_setaffinity(0, sizeof(mask), &mask) == -1) {
    printf("Set CPU affinity failue, ERROR:%s\n", strerror(errno));
    return -1;
  }

  for (; i < sizeof(mask); i++) {
    if (CPU_ISSET(i, &mask)) {
      printf("main: pid=%d,tid=%lu,bind cpu[%d]\n", getpid(), pthread_self(),
             i);
    }
  }

  while (true) {
    gSchedSetaffinityCount++;
    if (gSchedSetaffinityCount % (uint64_t(1e6)) == 0)
      printf("gSchedSetaffinityCount: %d\r\n", gSchedSetaffinityCount);
    sched_setaffinity(0, sizeof(mask), &mask);
  }

  while (1) {
    sleep(1);
  }

  return 0;
}
