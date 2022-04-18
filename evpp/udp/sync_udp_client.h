#include "evpp/inner_pre.h"

#include "udp_message.h"

namespace evpp {

// It is not asynchronous, please do not use it production.
// The only purpose it exists is for purpose of testing UDP Server.
class SyncUdpClient {
 public:
  SyncUdpClient(const SocketAddress& addr);
  SyncUdpClient();
  ~SyncUdpClient();

  void Close();
  bool Send(const std::string& msg);
  bool Send(const char* msg, size_t len);
  bool Connect(const SocketAddress& remote_addr);
  //! brief : Do a udp request and wait for remote udp server send response data
  //! param[in] - const std::string & udp_package_data
  //! return - std::string the response data
  std::string DoRequest(const std::string& udp_package_data,
                        uint32_t timeout_ms);

  static std::string DoRequest(const SocketAddress& remote_addr,
                               const std::string& udp_package_data,
                               uint32_t timeout_ms);

  static bool Send(const std::string& msg, const SocketAddress& remote_addr);
  static bool Send(const char* msg, size_t len,
                   const SocketAddress& remote_addr);
  // static bool Send(const MessagePtr& msg);
  // static bool Send(const Message* msg);

 public:
  evpp_socket_t sockfd() const { return sockfd_; }

 private:
  boost::optional<SocketAddress> local_addr_;
  SocketAddress remote_addr_;
  evpp_socket_t sockfd_ = -1;
  bool connected_ = false;
};

}  // namespace evpp
