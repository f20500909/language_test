#include "evpp/inner_pre.h"

#include "evpp/libevent.h"
#include "evpp/sockets.h"
#include "sync_udp_client.h"

namespace evpp {

SyncUdpClient::SyncUdpClient() : local_addr_(boost::none), sockfd_(INVALID_SOCKET) {
  memset(&remote_addr_, 0, sizeof(remote_addr_));
}

SyncUdpClient::SyncUdpClient(const SocketAddress& addr)
    : local_addr_(addr), sockfd_(INVALID_SOCKET) {
  memset(&remote_addr_, 0, sizeof(remote_addr_));
}

SyncUdpClient::~SyncUdpClient(void) { Close(); }

bool SyncUdpClient::Connect(const SocketAddress& remote_addr) {
  if(sockfd_ > 0){
      Close();
  }
  remote_addr_ = remote_addr;
  sockfd_ = ::socket(AF_INET, SOCK_DGRAM, 0);
  sock::SetReuseAddr(sockfd_);
  CHECK(remote_addr_.length() > 0);
  if (local_addr_) {
    if (::bind(sockfd_, local_addr_.get_ptr()->SocketAddr(),
               local_addr_.get_ptr()->length())) {
      int serrno = errno;
      LOG_ERROR << "socket bind error=" << serrno << " " << strerror(serrno);
      return INVALID_SOCKET;
    }
    LOG(INFO) << "udp Connect bind " << local_addr_.get_ptr()->ToString();
    sock::SetReusePort(sockfd_);
  }
  CHECK(remote_addr_.length() > 0);
  int ret = ::connect(sockfd_, remote_addr_.SocketAddr(), remote_addr_.length());

  if (ret != 0) {
    LOG_ERROR << "Failed to connect to remote=" << remote_addr.ToString()
              << " family=" << remote_addr.family() << ", errno=" << errno
              << " " << strerror(errno) << " len="<<remote_addr_.length();
    Close();
    return false;
  }

  connected_ = true;
  return true;
}

void SyncUdpClient::Close() { EVUTIL_CLOSESOCKET(sockfd_); }

std::string SyncUdpClient::DoRequest(const std::string& data, uint32_t timeout_ms) {
  if (!Send(data)) {
    int eno = errno;
    LOG_ERROR << "sent failed, errno=" << eno << " " << strerror(eno)
              << " , dlen=" << data.size();
    return "";
  }

  sock::SetTimeout(sockfd_, timeout_ms);

  size_t buf_size = 4096;  // The UDP max payload size
  UdpMessagePtr msg(new UdpMessage(sockfd_, buf_size));
  socklen_t addrLen = sizeof(SocketAddress);
  int readn = ::recvfrom(sockfd_, msg->WriteBegin(), buf_size, 0,
                         msg->RemoteAddr().SocketAddr(), &addrLen);
  int err = errno;
  if (readn >= 0) {
    msg->WriteBytes(readn);
    return std::string(msg->data(), msg->size());
  } else {
    LOG_ERROR << "errno=" << err << " " << strerror(err)
              << " recvfrom return -1";
  }

  return "";
}

std::string SyncUdpClient::DoRequest(const SocketAddress& remote_addr,
                              const std::string& udp_package_data,
                              uint32_t timeout_ms) {
  SyncUdpClient c;
  if (!c.Connect(remote_addr)) {
    return "";
  }

  return c.DoRequest(udp_package_data, timeout_ms);
}

bool SyncUdpClient::Send(const char* msg, size_t len) {
  if (connected_) {
    int sentn = ::send(sockfd(), msg, len, 0);
    return static_cast<size_t>(sentn) == len;
  }
  struct sockaddr* addr = remote_addr_.SocketAddr();
  socklen_t addrlen = remote_addr_.length();

  int sentn = ::sendto(sockfd(), msg, len, 0, addr, addrlen);
  return sentn > 0;
}

bool SyncUdpClient::Send(const std::string& msg) {
  return Send(msg.data(), msg.size());
}

bool SyncUdpClient::Send(const std::string& msg, const SocketAddress& addr) {
  return SyncUdpClient::Send(msg.data(), msg.size(), addr);
}

bool SyncUdpClient::Send(const char* msg, size_t len, const SocketAddress& addr) {
  SyncUdpClient c;
  if (!c.Connect(addr)) {
    return false;
  }

  return c.Send(msg, len);
}
#if 0
bool SyncUdpClient::Send(const MessagePtr& msg) {
  return SyncUdpClient::Send(msg->data(), msg->size(),SocketAddress(msg->remote_addr()));
}

bool SyncUdpClient::Send(const Message* msg) {
  return SyncUdpClient::Send(msg->data(), msg->size());
}
#endif

}  // namespace evpp
