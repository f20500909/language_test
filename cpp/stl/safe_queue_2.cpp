#include <condition_variable>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>

template <typename T>
class
    threadsafe_queue {  // 因为这个模板方法，所以方法定义就没必要和声明分开了！！！，不要搞混了。
  typedef std::queue<T> queue;  // using 不能用到模板上去
 private:
  mutable std::mutex mut;  // mutex不是一个模板
  std::queue<T> data_queue;
  std::condition_variable data_cond;

 public:
  threadsafe_queue() {}

  threadsafe_queue(threadsafe_queue const& other) {
    std::lock_guard<std::mutex> lk(other.mut);  // 必须锁住互斥元
    data_queue = other.data_queue;
  }

  void push(T new_value) {
    std::lock_guard<std::mutex> lk(mut);
    data_queue.push(new_value);
    data_cond .notify_one();  // 这句话的意思是，如果放了一个，就应该通知一个等待线程
  }

  void wait_and_pop(T& value) {
    std::unique_lock<std::mutex> lk(mut);
    data_cond.wait(lk, [this] { return data_queue.empty(); });
    value = data_queue.front();
    data_queue.pop();  // 为什么queue 和 stack 要把一个操作拆成两个呢，伤脑筋！
  }

  std::shared_ptr<T> wait_and_pop() {
    std::unique_lock<std::mutex> lk(mut);
    data_cond.wait(lk, [this] { return !data_queue.empty(); });
    std::shared_ptr<T> ret(std::make_shared<T>(data_queue.front()));
    data_queue.pop();
    return ret;
  }

  // 这个方法比较有趣，先去判断能不能拿到锁
  bool try_pop(T& value) {
    // +1尝试拿锁，拿不到就默认直接返回 false, 这里并没有写没有拿到锁该怎么办，
    // 应该是执行的时候会直接补全，bool的默认值就是false
    // ps: 这是错误的理解，
    // 一定要等待拿到锁才能往下执行，否则会稍微阻塞在这行代码里
    std::unique_lock<std::mutex> lk(mut);

    if (data_queue.empty()) return false;
    value = data_queue.front();
    data_queue.pop();
    return true;
  }

  std::shared_ptr<T> try_pop() {
    // 尝试拿锁，拿不到就默认直接返回一个空的共享指针
    std::unique_lock<std::mutex> lk(mut);

    if (data_queue.empty()) return false;

    // 这里记得要把队列头元素变成一个共享指针
    std::shared_ptr<T> ret = std::make_shared(data_queue.front());
    data_queue.pop();  // 先出队列，然后返回，如果先返回了，队列就没有
    return ret;
  }

  bool empty() const {
    // 这里非常有必要说一下，为了防止调用这个方法的时候有其他线程入队操作，必须锁住队列，这样虽然降低了并发，但是可靠
    std::lock_guard<std::mutex> lk(mut);
    return data_queue.empty();
  }
};

class A {
 public:
  int i1 = 1;
};

A& prepare_A() {
  A* a = new A();
  return *a;
}

#include <iostream>
void process_A(A& a) {
  std::cout << "process A " << std::endl;
  delete &a;
}

void produce_thread(threadsafe_queue<A> queue) {
  int i = 0;
  int stop = 10;
  while (i < stop) {
    A const a = prepare_A();
    queue.push(a);
    i++;
  }
}

void consume_thread(threadsafe_queue<A> queue) {
  int i = 0;
  int stop = 10;
  while (true) {
    A a;
    queue.wait_and_pop(a);
    process_A(a);
    if (i == stop) break;
    i++;
  }
}

void test_queue() {
  threadsafe_queue<A> queue;
  std::thread t1(produce_thread, std::ref(queue));
  std::thread t2(consume_thread, std::ref(queue));
  t1.join();
  t2.join();
}

int main() { test_queue(); }