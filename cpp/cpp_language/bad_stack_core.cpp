
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

#include <cstring>
#include <string>

// class A {
//  public:
//   int a = 0;
//   void print() { printf("a:%d", this->a); }

// };

int main() {
  char* buf = "sdfsdf";
  strncpy(buf, "[PSH] ", strlen("[PSH] "));

  char* dest_str;
  char* src_str = "source char string";
  strncpy(dest_str, src_str, sizeof(src_str));

  return 0;
}
