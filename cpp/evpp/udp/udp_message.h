#pragma once

#include "evpp/buffer.h"
#include "evpp/socket_address.h"
#include "evpp/sockets.h"
#include "evpp/sys_sockets.h"
namespace evpp {

class UdpMessage : public Buffer {
 public:
  UdpMessage(evpp_socket_t fd, size_t buffer_size = 1472)
      : Buffer(buffer_size), sockfd_(fd) {}

  void set_remote_addr(const SocketAddress& raddr) { remote_addr_ = raddr; }

  const SocketAddress& RemoteAddr() const noexcept { return remote_addr_; }
  SocketAddress& RemoteAddr() noexcept { return remote_addr_; }

  evpp_socket_t sockfd() const { return sockfd_; }

 private:
  mutable SocketAddress remote_addr_;
  int sockfd_;
};

typedef std::shared_ptr<UdpMessage> UdpMessagePtr;
typedef std::function<void(UdpMessagePtr& msg)> UdpMessageHandler;

inline bool SendMessage(evpp_socket_t fd, const SocketAddress& addr,
                        const char* d, size_t dlen) {
  if (dlen == 0) {
    return true;
  }

  int sentn = ::sendto(fd, d, dlen, 0, addr.SocketAddr(), addr.length());
  if (sentn != (int)dlen) {
    return false;
  }

  return true;
}

inline bool SendMessage(evpp_socket_t fd, const SocketAddress& addr,
                        const std::string& d) {
  return SendMessage(fd, addr, d.data(), d.size());
}

inline bool SendMessage(evpp_socket_t fd,
                        const SocketAddress& addr, const Slice& d) {
  return SendMessage(fd, addr, d.data(), d.size());
}

inline bool SendMessage(const UdpMessagePtr& msg) {
  return SendMessage(msg->sockfd(), msg->RemoteAddr(), msg->data(),
                     msg->size());
}

}  // namespace evpp