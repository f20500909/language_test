#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <queue>
#include "unistd.h"

#include "time.h"

//生产者数量
#define PRODUCT_SIZE 20
//消费者数量
#define CUSTOMER_SIZE 1
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
        usleep(10);
        std::unique_lock<std::mutex> lck(mut);
        while (que.size() > MAX_SIZE)
        {

            con.wait(lck);
        }
        int data = rand();
        que.push(data);
        std::cout << std::this_thread::get_id() << "do produce：" << data << std::endl;
        con.notify_all();
    }
}
void Customer()
{
    while (true)
    {
        std::unique_lock<std::mutex> lck(mut);
        while (que.empty())
        {
            con.wait(lck);
        }
        std::cout << std::this_thread::get_id() << "do consume：" << que.front() << std::endl;
        que.pop();
        con.notify_all();
    }
}
int main()
{
    std::vector<std::thread> threadPoll;
    //创建生产者和消费者
    for (int i = 0; i < PRODUCT_SIZE; ++i)
    {
        threadPoll.push_back(std::thread(Producter));
    }
    for (int i = 0; i < PRODUCT_SIZE + CUSTOMER_SIZE; ++i)
    {
        threadPoll.push_back(std::thread(Customer));
    }

    for (int i = 0; i < PRODUCT_SIZE + CUSTOMER_SIZE; ++i)
    {
        threadPoll[i].join();
    }

    return 0;
}


