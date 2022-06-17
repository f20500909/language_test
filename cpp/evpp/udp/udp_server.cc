#include "evpp/udp/udp_server.h"
#include <memory>
#include "evpp/event_loop.h"
#include "evpp/inner_pre.h"
#include "evpp/libevent.h"
#include "evpp/sockets.h"
#include <sys/ioctl.h>
using namespace evpp;

void defaultUdpMessageCallback(const UdpEndpointPtr &ptr, UdpMessagePtr &buf) {
  std::string msg = buf->NextAllString();
  LOG_INFO << "defaultUdpMessageCallback:" << msg;
}

evpp_socket_t CreateUdpServer(const SocketAddress &addr) {
  evpp_socket_t fd = ::socket(AF_INET, SOCK_DGRAM, 0);
  if (fd == -1) {
    int serrno = errno;
    LOG(ERROR) << "socket error " << evpp::strerror(serrno);
    return INVALID_SOCKET;
  }
  sock::SetReuseAddr(fd);
  sock::SetReusePort(fd);

  if (::bind(fd, addr.SocketAddr(), addr.length())) {
    int serrno = errno;
    LOG(ERROR) << "socket bind error=" << serrno << " "
               << evpp::strerror(serrno);
    return INVALID_SOCKET;
  }

  return fd;
}

UdpEndpoint::UdpEndpoint(EventLoop *loop, const SocketAddress &addr,
                         const std::string &name)
    : loop_(loop),
      local_addr_(addr),
      fd_(CreateUdpServer(addr)),
      channel_(new FdChannel(loop, fd_,true,false)),
      name_(name) {
  channel_->SetReadCallback(
      std::bind(&UdpEndpoint::HandleRead, this));
  //loop_->RunInLoop([this]() { channel_->EnableReadEvent(); });
}

UdpEndpoint::~UdpEndpoint() {
  LOG_INFO << "UdpEndpoint::dtor[" << name_ << "] at " << this
           << " fd=" << channel_->fd();

  if (fd_ >= 0) {
    assert(channel_);
    assert(fd_ == channel_->fd());
    assert(channel_->IsNoneEvent());
    EVUTIL_CLOSESOCKET(fd_);
    fd_ = INVALID_SOCKET;
  }
}

void UdpEndpoint::Close() {
  auto ptr = shared_from_this();
  loop_->QueueInLoop([ptr]() {
    ptr->channel_->DisableAllEvent();
    ptr->channel_->Close();
    ptr->channel_.reset();
  });
}

void UdpEndpoint::HandleRead() {
  while (1) {
    int rcvdBytes = 0;
    int ret = ::ioctl(fd_, FIONREAD, &rcvdBytes); /*unix */
    if (ret <= 0) {
      return;
    }
    UdpMessagePtr recv_msg(new UdpMessage(fd_, rcvdBytes));
    socklen_t addr_len = sizeof(SocketAddress);
    int readn = ::recvfrom(fd_, (char *)recv_msg->WriteBegin(), rcvdBytes, 0,
                           recv_msg->RemoteAddr().SocketAddr(), &addr_len);
    if (readn >= 0) {
      LOG_TRACE << "fd=" << fd_ << " addr=" <<local_addr_.ToString()
                << " recv len=" << readn << " from "
                << recv_msg->RemoteAddr().ToString();
      recv_msg->WriteBytes(readn);
      callback_(recv_msg);
    } else {
      int eno = errno;
      if (EVUTIL_ERR_RW_RETRIABLE(eno)) {
        continue;
      }
      LOG(ERROR) << "errno=" << eno << " " << evpp::strerror(eno);
    }
  }
}


bool UdpEndpoint::Send(const SocketAddress &remote, const char *msg,
                       size_t len) {
  CHECK(fd_ > 0);
  int sentn = ::sendto(fd_, msg, len, 0, remote.SocketAddr(),
                       remote.length());
  return sentn > 0;
}

bool UdpEndpoint::Send(const SocketAddress &remote, const std::string &str) {
  return Send(remote, str.c_str(), str.length());
}

UdpServer::UdpServer() {}

void UdpServer::Start(int thread_num) {
  CHECK(thread_num > 0);
  if (started_.load() == false) {
    started_.store(true);
    thread_pool_ = std::make_shared<EventLoopThreadPool>(nullptr, thread_num);
    thread_pool_->Start(true);
  }
}
void UdpServer::Stop() {
  //TODO ?
  
}

UdpEndpointPtr UdpServer::MakeEndpoint(const SocketAddress &addr,
                                       const std::string &name,
                                       UdpMessageHandler &&handle) {
  std::lock_guard<std::mutex> lock(mutex_);
  EventLoop *loop = thread_pool_->GetNextLoop();
  UdpEndpointPtr ptr = std::make_shared<UdpEndpoint>(loop, addr, name);
  ptr->SetMessageCallback(std::move(handle));
  endpoints_.insert(ptr);
  LOG(INFO) << "andUdpEndpoint  name:" << name << "addr:" << addr.ToString();
  return ptr;
}

void UdpServer::RemoveEndpoint(const UdpEndpointPtr &ptr) {
  std::lock_guard<std::mutex> lock(mutex_);
  ptr->Close();
  endpoints_.erase(ptr);
}
