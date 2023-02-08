#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

#include "time.h"
#include "unistd.h"

// 生产者数量
#define PRODUCT_SIZE 2
// 消费者数量
#define CUSTOMER_SIZE 2
// 最大产品数量
#define MAX_SIZE 10

// 互斥锁
std::mutex mtx;
// 条件变量
std::condition_variable con;
// 队列，模拟缓冲区
std::queue<int> q;
void Producter() {
  while (true) {
    std::unique_lock<std::mutex> lk(mtx);
    while (q.size() > MAX_SIZE) {
      con.wait(lk);
    }
    static int g_id = 1;
    q.push(g_id++);
    usleep(100);
    std::cout << std::this_thread::get_id()
              << "=================== do produce：" << g_id << std::endl;
    con.notify_all();
  }
}

void Customer() {
  while (true) {
    std::unique_lock<std::mutex> lk(mtx);
    while (q.empty()) {
      con.wait(lk);
    }
    std::cout << std::this_thread::get_id() << " do consume：" << q.front()
              << std::endl;
    q.pop();
    con.notify_all();
  }
}

int main() {
  std::vector<std::thread> threadPoll;
  // 创建生产者和消费者
  for (int i = 0; i < PRODUCT_SIZE; ++i) {
    threadPoll.push_back(std::thread(Producter));
  }
  for (int i = 0; i < PRODUCT_SIZE + CUSTOMER_SIZE; ++i) {
    threadPoll.push_back(std::thread(Customer));
  }

  for (int i = 0; i < PRODUCT_SIZE + CUSTOMER_SIZE; ++i) {
    threadPoll[i].join();
  }

  return 0;
}


