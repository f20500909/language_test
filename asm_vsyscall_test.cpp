/*
 *Copyright (c) 2013 GuZheng
 * Author: GuZheng <cengku@gmail.com>
 * Demo of how to use vsyscall func
 *
 * */
/* vsyscall.h header info
#ifndef _ASM_X86_VSYSCALL_H
#define _ASM_X86_VSYSCALL_H

enum vsyscall_num {
        __NR_vgettimeofday,
        __NR_vtime,
        __NR_vgetcpu,
};

#define VSYSCALL_START (-10UL << 20)
#define VSYSCALL_SIZE 1024
#define VSYSCALL_END (-2UL << 20)
#define VSYSCALL_MAPPED_PAGES 1
#define (vsyscall_nr) (VSYSCALL_START+VSYSCALL_SIZE*(vsyscall_nr))


#endif
*/

#include <asm/vsyscall.h> /*the vsyscall header*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <sys/syscall.h>


struct getcpu_cache
{
    unsigned long blob[128 / sizeof(long)];
};

int getVDSOTime()
{
    /*这个内核版本有三个vsyscall实现的系统调用
     *gettimeofday,time和getcpu
     *因此先定义三个函数原型的指针，用来接受后面找到的函数的映射地址
     * */

    int (*my_getcpu)(unsigned *cpu, unsigned *node, struct getcpu_cache *tcache);
    int (*my_gettimeofday)(struct timeval * tv, struct timezone * tz);
    time_t (*my_time)(time_t * calptr);

    unsigned node, cpu;
    time_t tm;
    struct tm *gm;
    struct timeval tv;
    struct timezone tz;
    struct getcpu_cache cache;

    /*根据VSYSCALL_ADDR宏，以及三个函数的偏移就可以找到映射的地址了*/
    char *addr1 = (char *)VSYSCALL_ADDR(__NR_vgettimeofday);
    // printf("vgettimeofday addr is %p \n", addr1);


    my_gettimeofday = (int (*)(struct timeval *, struct timezone *))addr1;

    // my_gettimeofday = (int (*)(struct timeval *, struct timezone *))0xffffffffff600000;
	printf("run line 68\n");
    return my_gettimeofday(&tv, &tz);
         
    // return  syscall(SYS_gettimeofday,&tv, &tz);
}

int funCRow()
{
    /*这个内核版本有三个vsyscall实现的系统调用
     *gettimeofday,time和getcpu
     *因此先定义三个函数原型的指针，用来接受后面找到的函数的映射地址
     * */
    int (*my_getcpu)(unsigned *cpu, unsigned *node, struct getcpu_cache *tcache);
    int (*my_gettimeofday)(struct timeval * tv, struct timezone * tz);
    time_t (*my_time)(time_t * calptr);

    unsigned node, cpu;
    time_t tm;
    struct tm *gm;
    struct timeval tv;
    struct timezone tz;
    struct getcpu_cache cache;

    /*根据VSYSCALL_ADDR宏，以及三个函数的偏移就可以找到映射的地址了*/
    char *addr1 = (char *)VSYSCALL_ADDR(__NR_vgettimeofday);
    printf("vgettimeofday addr is%p \n", addr1);
    char *addr2 = (char *)VSYSCALL_ADDR(__NR_vtime);
    printf("vtime addr is %p \n", addr2);
    char *addr3 = (char *)VSYSCALL_ADDR(__NR_vgetcpu);
    printf("vgetcpu addr is %p \n", addr3);

    my_gettimeofday = (int (*)(struct timeval *, struct timezone *))addr1;

    // while (true)
    // {
    //     // my_gettimeofday(NULL, NULL);
    //     // gettimeofday(&tv, &tz);
    // }


    my_time = (time_t(*)(time_t *))addr2;
    // time(&tm);

    gm = gmtime(&tm);
    printf("Now is %d-%d-%d %d:%d:%d\n",
           gm->tm_year + 1900,
           gm->tm_mon,
           gm->tm_mday,
           gm->tm_hour,
           gm->tm_min,
           gm->tm_sec);

    my_getcpu = (int (*)(unsigned *, unsigned *, struct getcpu_cache *))addr3;
    my_getcpu(&cpu, &node, &cache);

    printf("numa node:%d, run on cpu:%d\n", node, cpu);
}

int main(int argc, char **argv)
{
    // funCRow();
    while(true) 
    getVDSOTime();
    return 0;
}