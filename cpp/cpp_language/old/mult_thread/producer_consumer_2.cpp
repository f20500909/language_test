#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

#include "time.h"
#include "unistd.h"

// 最大产品数量
#define MAX_SIZE 10

// 互斥锁
std::mutex mut;
// 条件变量
std::condition_variable con;
// 队列，模拟缓冲区
std::queue<int> q;

void Producter() {
  while (true) {
    std::unique_lock<std::mutex> lk(mut);
    while (q.size() > 100) {
      con.wait(lk);
    }

    static int g_cnt = 0;
    q.push(g_cnt);
    std::cout << " do producter: " << g_cnt << std::endl;
    ++g_cnt;
    con.notify_all();
  }
}

void Customer() {
  while (true) {
    std::unique_lock<std::mutex> lk(mut);
    while (q.empty()) {
      con.wait(lk);
    }

    int v = q.front();
    q.pop();
    std::cout << " do customer: " << v << std::endl;
    con.notify_all();
  }
}

int main() {
  std::vector<std::thread> threadPoll;
  // 创建生产者和消费者
  std::vector<std::thread> thread_poll;
  for (int i = 0; i < 2; i++) {
    thread_poll.push_back(std::thread(Producter));
    thread_poll.push_back(std::thread(Customer));
  }

  for (int i = 0; i < thread_poll.size(); i++) {
    thread_poll[i].join();
  }

  return 0;
}
