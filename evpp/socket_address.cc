#include "evpp/socket_address.h"
#include "evpp/inner_pre.h"
#include "evpp/libevent.h"
#include "evpp/logging.h"
#include "src/base/string_to_number.h"

namespace evpp {

static const in6_addr kV4MappedPrefix = {
    {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xFF, 0xFF, 0}}};
static const in6_addr k6To4Prefix = {{{0x20, 0x02, 0}}};
static const in6_addr kTeredoPrefix = {{{0x20, 0x01, 0x00, 0x00}}};
static const in6_addr kV4CompatibilityPrefix = {{{0}}};
static const in6_addr k6BonePrefix = {{{0x3f, 0xfe, 0}}};
static const in6_addr kPrivateNetworkPrefix = {{{0xFD}}};

SocketAddress::SocketAddress() {
  static_assert(sizeof(SocketAddress) == sizeof(sockaddr_storage),
                "sizeof SocketAddress  == sockaddr_storage");
  Clear();
}

SocketAddress::SocketAddress(const sockaddr_in& in4) {
  Clear();
  std::memcpy(&ss_, &in4, sizeof(sockaddr_in));
}

SocketAddress::SocketAddress(const sockaddr_in6& in6) {
  Clear();
  std::memcpy(&ss_, &in6, sizeof(sockaddr_in6));
}

SocketAddress::SocketAddress(uint32_t ip_as_host_order_integer, uint16_t port) {
  Clear();
  SetIP(ip_as_host_order_integer);
  SetPort(port);
}

SocketAddress::SocketAddress(const struct in_addr& ip4, uint16_t port) {
  Clear();
  SetIP(ip4);
  SetPort(port);
}

SocketAddress::SocketAddress(const struct in6_addr& ip6, uint16_t port) {
  Clear();
  SetIP(ip6);
  SetPort(port);
}

SocketAddress::SocketAddress(const SocketAddress& addr) {
  this->operator=(addr);
}

void SocketAddress::Clear() { memset(&ss_, 0, sizeof(sockaddr_storage)); }

bool SocketAddress::IsNil() const {
  return ss_.ss_family == AF_UNSPEC && 0 == port();
}

SocketAddress& SocketAddress::operator=(const SocketAddress& addr) {
  ss_ = addr.ss_;
  return *this;
}

void SocketAddress::SetIP(uint32_t ip_as_host_order_integer) {
  ss_.ss_family = AF_INET;
  struct sockaddr_in* addr4 = sock::sockaddr_in_cast(&ss_);
  addr4->sin_addr.s_addr = htonl(ip_as_host_order_integer);
}

void SocketAddress::SetIP(const struct in_addr& in4) {
  ss_.ss_family = AF_INET;
  struct sockaddr_in* addr4 = sock::sockaddr_in_cast(&ss_);
  memcpy(&addr4->sin_addr, &in4, sizeof(struct in_addr));
}

void SocketAddress::SetIP(const struct in6_addr& ip6) {
  ss_.ss_family = AF_INET6;
  struct sockaddr_in6* addr6 = sock::sockaddr_in6_cast(&ss_);
  memcpy(&addr6->sin6_addr, &ip6, sizeof(struct in6_addr));
}

void SocketAddress::SetPort(uint16_t port) {
  struct sockaddr_in* addr4 =
      const_cast<struct sockaddr_in*>(sock::sockaddr_in_cast(&ss_));
  addr4->sin_port = htons(port);
}

uint32_t SocketAddress::ip() const {
  if (ss_.ss_family == AF_INET) {
    const sockaddr_in* addr4 = sock::sockaddr_in_cast(&ss_);
    return ntohl(addr4->sin_addr.s_addr);
  }
  return 0;
}

in_addr* SocketAddress::ipv4_address() noexcept {
  if (ss_.ss_family == AF_INET) {
    return &sock::sockaddr_in_cast(&ss_)->sin_addr;
  }
  return nullptr;
}

in6_addr* SocketAddress::ipv6_address() noexcept {
  if (ss_.ss_family == AF_INET) {
    return &sock::sockaddr_in6_cast(&ss_)->sin6_addr;
  }
  return nullptr;
}
#if 1
const in_addr* SocketAddress::ipv4_address() const noexcept {
  if (ss_.ss_family == AF_INET) {
    return &sock::sockaddr_in_cast(&ss_)->sin_addr;
  }
  return nullptr;
}

const in6_addr* SocketAddress::ipv6_address() const noexcept {
  if (ss_.ss_family == AF_INET) {
    return &sock::sockaddr_in6_cast(&ss_)->sin6_addr;
  }
  return nullptr;
}
#endif

uint16_t SocketAddress::port() const {
  if (ss_.ss_family == AF_INET) {
    return ntohs(sock::sockaddr_in_cast(&ss_)->sin_port);
  } else {
    return ntohs(sock::sockaddr_in6_cast(&ss_)->sin6_port);
  }
  return 0;
}
int SocketAddress::length() const noexcept {
  if (ss_.ss_family == AF_INET) {
    return sizeof(struct sockaddr_in);
  } else if (ss_.ss_family == AF_INET6)  {
    return sizeof(struct sockaddr_in6);
  }
  return 0;
}
uint32_t SocketAddress::scope_id() const {
  if (ss_.ss_family == AF_INET6) {
    return sock::sockaddr_in6_cast(&ss_)->sin6_scope_id;
  }
  return 0;
}
void SocketAddress::SetScopeID(uint32_t id) {
  if (ss_.ss_family == AF_INET6) {
    sock::sockaddr_in6_cast(&ss_)->sin6_scope_id = id;
  }
}
std::string SocketAddress::ToString() const { return sock::ToIPPort(&ss_); }
std::string SocketAddress::ToIPString() const {
  return sock::ToIP(sock::sockaddr_cast(&ss_));
}
bool SocketAddress::IsAnyIP() const {
  switch (family()) {
    case AF_INET:
      return sock::IsEqualIp(ss_, EmptySocketAddressWithFamily(AF_INET).ss_);
    case AF_INET6:
      return sock::IsEqualIp(ss_, EmptySocketAddressWithFamily(AF_INET6).ss_) ||
             sock::IsEqualIp(ss_, SocketAddress(kV4MappedPrefix, 0).ss_);
    case AF_UNSPEC:
      return false;
  }
  return false;
}

bool SocketAddress::IsLoopbackIP() const {
  switch (family()) {
    case AF_INET: {
      uint32_t ip_in_host_order = ip();
      return ((ip_in_host_order >> 24) == 127);
    }
    case AF_INET6:
      return sock::IsEqualIp(ss_, SocketAddress(in6addr_loopback, 0).ss_);
    default:
      return false;
  }
  return false;
}

bool SocketAddress::IsPrivateIP() const {
  uint32_t temp = ip();
  return ((temp >> 24) == 10) || ((temp >> 20) == ((172 << 4) | 1)) ||
         ((temp >> 16) == ((192 << 8) | 168));
}

bool SocketAddress::operator==(const SocketAddress& addr) const {
  return sock::IsEqual(ss_, addr.ss_);
}

bool SocketAddress::operator<(const SocketAddress& other) const {
  // IPv4 is 'less than' IPv6
  if (ss_.ss_family != other.ss_.ss_family) {
    if (ss_.ss_family == AF_UNSPEC) {
      return true;
    }
    if (ss_.ss_family == AF_INET && other.ss_.ss_family == AF_INET6) {
      return true;
    }
    return false;
  }
  // Comparing addresses of the same family.
  switch (ss_.ss_family) {
    case AF_INET: {
      return this->ip() < other.ip();
    }
    case AF_INET6: {
      return ::memcmp(this->ipv6_address(), other.ipv6_address(),
                      sizeof(in6_addr)) < 0;
    }
  }
  return this->port() < other.port();
}

size_t SocketAddress::Hash() const {
  size_t h = 0;
  switch (ss_.ss_family) {
    case AF_INET: {
      uint32_t ipnetorder = ipv4_address()->s_addr;
      h ^= ipnetorder;
    } break;
    case AF_INET6: {
      in6_addr v6addr = *(this->ipv6_address());
      const uint32_t* v6_as_ints =
          reinterpret_cast<const uint32_t*>(&v6addr.s6_addr);
      h ^= v6_as_ints[0] ^ v6_as_ints[1] ^ v6_as_ints[2] ^ v6_as_ints[3];
    } break;
    default: {
      LOG(WARNING) << " unsopport sin_family= " << (int)ss_.ss_family;
    }
  }
  uint16_t p = port();
  h ^= p | (p << 16);
  return h;
}

boost::optional<SocketAddress> MakeSocketAddress(const std::string& str) {
  if (str.empty()) {
    return boost::none;
  }
  SocketAddress addr;
  if (str[0] == '[') {
    // IPV6
    size_t closebracket = str.rfind(']');
    if (closebracket == std::string::npos) {
      return boost::none;
    }
    size_t colon = str.find(':', closebracket);
    if (colon == std::string::npos || colon == closebracket) {
      return boost::none;
    }
    boost::optional<uint16_t> port_op =
        mswitch::StringToNumber<uint16_t>(str.substr(colon + 1));
    if (!port_op) {
      LOG(WARNING) << "parse port failed";
      return boost::none;
    }
    addr.SetPort(port_op.value());
    addr.ss_.ss_family = AF_INET6;
    if (::evutil_inet_pton(AF_INET6, str.substr(1, closebracket - 1).c_str(),
                           addr.ipv6_address()) == 0) {
      return boost::none;
    }
    return addr;
  }
  size_t pos = str.rfind(':');
  if (pos == std::string::npos || pos == str.size() - 1) {
    LOG(WARNING) << "Address specified error <" << str << ">. Cannot find port";
    return boost::none;
  }
  boost::optional<uint16_t> port_op =
      mswitch::StringToNumber<uint16_t>(str.substr(pos + 1));
  if (!port_op) {
    LOG(WARNING) << "Address specified error <" << str << ">. Cannot find port";
    return boost::none;
  }
  addr.ss_.ss_family = AF_INET;
  if (::evutil_inet_pton(AF_INET, str.substr(0, pos).c_str(),
                         addr.ipv4_address()) == 0) {
    return boost::none;
  }
  addr.SetPort(port_op.value());
  return addr;
} 

SocketAddress EmptySocketAddressWithFamily(int family) {
  if (family == AF_INET) {
    return SocketAddress(INADDR_ANY, 0);
  } else if (family == AF_INET6) {
    return SocketAddress(in6addr_any, 0);
  }
  return SocketAddress();
}

}  // namespace evpp
