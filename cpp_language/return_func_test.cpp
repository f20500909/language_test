
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



unsigned int no_return_value_func()
{
    printf("test_no_return_value \r\n");
}

int main()
{
    auto a = no_return_value_func();
    printf("int a=%u\r\n",a);

    return 0;
}