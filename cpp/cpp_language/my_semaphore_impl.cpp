#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

class semaphore {
 public:
  semaphore(long count = 0) : count(count) {}
  void wait() {
    std::unique_lock<std::mutex> lock(mx);
    cond.wait(lock, [&]() { return count > 0; });
    --count;
  }
  void signal() {
    std::unique_lock<std::mutex> lock(mx);
    ++count;
    cond.notify_one();
  }

 private:
  std::mutex mx;
  std::condition_variable cond;
  long count;
};

semaphore sem(0);

void func1() {
  // do something
  while (true) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "func1" << std::endl;
    sem.signal();
  }
}

void func2() {
  while (true) {
    sem.wait();
    // do something
    std::cout << "func2" << std::endl;
  }
}

int main() {
  std::thread thread1(func1);
  std::thread thread2(func2);
  if (thread1.joinable()) thread1.join();
  if (thread2.joinable()) thread2.join();
}
