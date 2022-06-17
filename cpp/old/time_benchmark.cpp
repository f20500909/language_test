#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/syscall.h>

int main(int argc, char **argv)
{
        unsigned long i = 0;
        time_t (*f)(time_t *) = (time_t(*)(time_t *))0xffffffffff600400UL;

        if (!strcmp(argv[1], "vsyscall"))
        {
                // while(true)
                for (i = 0; i < 10000000; ++i)
                        f(NULL);
        }
        else if (!strcmp(argv[1], "vdso"))
        {
                // while(true)
                for (i = 0; i < 10000000; ++i)
                        time(NULL);
        }
        else
        {
                // while (true)
                for (i = 0; i < 100000000; ++i)
                {
                        syscall(SYS_time, NULL);
                        // int data[100][100] = {};
                }
        }

        // return 0;
}