
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

class A {
 public:
  int a =0;
  void print() { printf("a:%d", this->a); }
};

int main() {
  A a;
  a.print();

  return 0;
}
