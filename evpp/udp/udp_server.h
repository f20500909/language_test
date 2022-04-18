#pragma once

#include "evpp/event_loop_thread_pool.h"
#include "evpp/fd_channel.h"
#include "evpp/inner_pre.h"
#include "evpp/thread_dispatch_policy.h"
#include "udp_message.h"

#include <thread>
#include <set>

namespace evpp {

class EventLoop;

class UdpEndpoint : public std::enable_shared_from_this<UdpEndpoint> {
 public:
  // static UdpEndpointPtr create(InetAddress &addr,std::string& name);
  UdpEndpoint(EventLoop *loop, const SocketAddress &addr, const std::string &name);
  ~UdpEndpoint();
  EventLoop *loop() { return loop_; }
  const std::string &name() const noexcept { return name_; }

  bool Send(const SocketAddress &remote, const char *data, size_t len);
  bool Send(const SocketAddress &remote, const std::string &str);
  bool Send(const struct sockaddr_storage &remote, const std::string &str);
  void SetMessageCallback(UdpMessageHandler &&cb) { callback_ = std::move(cb); }
  void Close();
 private:
  void HandleRead();

  EventLoop *loop_;
  SocketAddress local_addr_;
  int fd_;
  std::unique_ptr<FdChannel> channel_;
  std::string name_;

  UdpMessageHandler callback_;
  friend class UdpServer;
};

typedef std::shared_ptr<UdpEndpoint> UdpEndpointPtr;
class UdpServer {
 public:
  UdpServer();
  ~UdpServer();

  void Start(int thread_num);
  void Stop();
  UdpEndpointPtr MakeEndpoint(const SocketAddress &add, const std::string &name,UdpMessageHandler&& handle);
  void RemoveEndpoint(const UdpEndpointPtr &ptr);

 private:
  // callback
  std::atomic<bool> started_{false};
  std::shared_ptr<EventLoopThreadPool> thread_pool_;

  std::mutex mutex_;  // lock endpoints_
  std::set<UdpEndpointPtr> endpoints_;
};

}  // namespace evpp