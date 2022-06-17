
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
#define OID_GET_HO_NBR                          "1.3.6.1.4.1.2702.1.43.1.92"

int main()
{

    char str[512];

    

    sprintf(str, "%s.%u", str, 1);
    sprintf(str, "%s",OID_GET_HO_NBR);


    // sprintf(str, "%s.%s", str, "test....");

    printf("res: %s", str);

    return 0;
}
