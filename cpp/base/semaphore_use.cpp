#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <iostream>

using namespace std;
sem_t sem;

void* change_resource(void* res) {
  char* msg = (char*)res;

  cout << "111" << endl;
  ;
  while (1) {
    sem_wait(&sem);
    strcat(msg, "a");
    printf("resource changed with value: %s\n", msg);
  }
}

int main() {
  int ret = sem_init(&sem, 0, 0);
  char* resource = "fdsfsd";
  pthread_t thread;
  if (ret == -1) {
    printf("sem_init failed \n");
    return -1;
  }
  cout << "sem_init ret :" << ret;

  ret = pthread_create(&thread, NULL, change_resource, (void*)resource);
  if (ret != 0) {
    printf("pthread_create failed \n");
    return -1;
  }

  while (1) {
    sem_post(&sem);
    sleep(1);
  }
}
