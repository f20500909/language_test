#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <queue>
#include "unistd.h"

#include "time.h"

//最大产品数量
#define MAX_SIZE 10

//互斥锁
std::mutex mut;
//条件变量
std::condition_variable con;
//队列，模拟缓冲区
std::queue<int> que;

void Producter()
{
    while (true)
    {
        usleep(1000000);
        std::lock_guard<std::mutex> lk(mut);

        static int g_cnt = 0;
        std::cout << " do producter: " << g_cnt << std::endl;
        ++g_cnt;
    }
}

void Customer()
{
    while (true)
    {
        if (que.empty())
            return;
        std::lock_guard<std::mutex> lk(mut);
        int v = que.front();
        que.pop();
        std::cout << " do customer: " << v << std::endl;
    }
}

int main()
{
    std::vector<std::thread> threadPoll;
    //创建生产者和消费者
    std::vector<std::thread> thread_poll;
    for (int i = 0; i < 2; i++)
    {
        thread_poll.push_back(std::thread(Producter));
        thread_poll.push_back(std::thread(Customer));
    }

    for (int i = 0; i < thread_poll.size(); i++)
    {
        thread_poll[i].join();
    }

    return 0;
}
