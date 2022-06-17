#ifndef MUDUO_NET_UDP_EVENTLOOPTHREADPOOL_H
#define MUDUO_NET_UDP_EVENTLOOPTHREADPOOL_H

#include <functional>
#include <memory>
#include <vector>

namespace muduo
{

namespace net
{

class EventLoop;
class EventLoopThread;

class UdpEventLoopThreadPool 
{
 public:
  typedef std::function<void(EventLoop*)> ThreadInitCallback;

  UdpEventLoopThreadPool(const std::string& nameArg);
  ~UdpEventLoopThreadPool();
  void setThreadNum(int numThreads) { numThreads_ = numThreads; }
  void start(const ThreadInitCallback& cb = ThreadInitCallback());

  // valid after calling start()
  /// round-robin
  EventLoop* getNextLoop();

  /// with the same hash code, it will always return the same EventLoop
  EventLoop* getLoopForHash(size_t hashCode);

  std::vector<EventLoop*> getAllLoops();

  bool started() const
  { return started_; }

  const std::string& name() const
  { return name_; }

 private:
  std::string name_;
  bool started_;
  int numThreads_;
  int next_;
  std::vector<std::unique_ptr<EventLoopThread>> threads_;
  std::vector<EventLoop*> loops_;
};

}  // namespace net
}  // namespace muduo

#endif  // MUDUO_NET_EVENTLOOPTHREADPOOL_H
