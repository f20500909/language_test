#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <pthread.h>
using namespace std;


class Mutex
{
public:
    Mutex()
    {
        pthread_mutex_init(&m_mutex, NULL);
    }
    ~Mutex()
    {
        pthread_mutex_destroy(&m_mutex);
    }
    void Enter()
    {
        pthread_mutex_lock(&m_mutex);
    }
    void Leave()
    {
        pthread_mutex_unlock(&m_mutex);
    }


private:
    pthread_mutex_t m_mutex;
};


class Guard
{
public:
    //使用引用，不然会发生拷贝构造函数
    Guard(Mutex &m):m_mutex(m)
    {
        //printf("in class m = %p\n", &m);
        m_mutex.Enter();
    }
    ~Guard()
    {
        m_mutex.Leave();
    }


private:
    //使用引用
    Mutex &m_mutex;
};


Mutex m;


void *f1(void *arg)
{
//    Guard guard(m);
    //printf("in func m = %p\n", &m);
    while (1)
    {
//        sleep(1);
        printf("str = %s\n", (char*)arg);
    }
}




int main()
{
    printf("---------------\n");
    pthread_t ntid1;
    pthread_t ntid2;

    char *ch[] = {"AAA", "BBB"};
    int err1 = pthread_create(&ntid1, NULL, f1, ch[0]);
    int err2 = pthread_create(&ntid2, NULL, f1, ch[1]);



    printf("T....................T\n");
    return 0;
}
