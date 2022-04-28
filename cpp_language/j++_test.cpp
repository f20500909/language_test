
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
#include <iostream>

using namespace std;

int main()
{
    static int j = 0;
     j++;

    std::cout << "[res] j = " << j << endl;
    size_t len =0;
    printf("len:%zu \n", len + 1);


    return 0;
}
