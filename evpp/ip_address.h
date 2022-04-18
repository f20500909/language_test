/*
 *  Copyright 2011 The WebRTC Project Authors. All rights reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */
#pragma once
#include "evpp/sockets.h"
#include <boost/optional.hpp>
#include <boost/utility/string_view.hpp>
namespace evpp {
// Version-agnostic IP address class, wraps a union of in_addr and in6_addr.
class IPAddress {
 public:
  IPAddress() : family_(AF_UNSPEC) { ::memset(&u_, 0, sizeof(u_)); }

  explicit IPAddress(const in_addr& ip4) : family_(AF_INET) {
    memset(&u_, 0, sizeof(u_));
    u_.ip4 = ip4;
  }

  explicit IPAddress(const in6_addr& ip6) : family_(AF_INET6) { u_.ip6 = ip6; }

  explicit IPAddress(uint32_t ip_in_host_byte_order) : family_(AF_INET) {
    memset(&u_, 0, sizeof(u_));
    u_.ip4.s_addr = ::htonl(ip_in_host_byte_order);
  }

  IPAddress(const IPAddress& other) : family_(other.family_) {
    ::memcpy(&u_, &other.u_, sizeof(u_));
  }

  virtual ~IPAddress() {}

  const IPAddress& operator=(const IPAddress& other) {
    family_ = other.family_;
    ::memcpy(&u_, &other.u_, sizeof(u_));
    return *this;
  }

  bool operator==(const IPAddress& other) const;
  bool operator!=(const IPAddress& other) const;
  bool operator<(const IPAddress& other) const;
  bool operator>(const IPAddress& other) const;


  inline std::ostream& operator<<(  // no-presubmit-check TODO(webrtc:8982)
      std::ostream& os) {           // no-presubmit-check TODO(webrtc:8982)
    return os << ToString();
  }

  int family() const { return family_; }
  in_addr ipv4_address() const;
  in6_addr ipv6_address() const;

  // Returns the number of bytes needed to store the raw address.
  size_t Size() const;

  // Wraps inet_ntop.
  std::string ToString() const;

  // Same as ToString but anonymizes it by hiding the last part.
  std::string ToSensitiveString() const;

  // Returns an unmapped address from a possibly-mapped address.
  // Returns the same address if this isn't a mapped address.
  IPAddress Normalized() const;

  // Returns this address as an IPv6 address.
  // Maps v4 addresses (as ::ffff:a.b.c.d), returns v6 addresses unchanged.
  IPAddress AsIPv6Address() const;

  // For socketaddress' benefit. Returns the IP in host byte order.
  uint32_t v4AddressAsHostOrderInteger() const;

  // Get the network layer overhead per packet based on the IP address family.
  int overhead() const;

  // Whether this is an unspecified IP address.
  bool IsNil() const;

 public:
  int family_;
  union {
    in_addr ip4;
    in6_addr ip6;
  } u_;
};



boost::optional<IPAddress> MakeIPAddress(const std::string& str);
boost::optional<IPAddress> MakeIPAddress(const addrinfo& info);

bool IPIsAny(const IPAddress& ip);
bool IPIsLoopback(const IPAddress& ip);
bool IPIsLinkLocal(const IPAddress& ip);
// Identify a private network address like "192.168.111.222"
// (see https://en.wikipedia.org/wiki/Private_network )
bool IPIsPrivateNetwork(const IPAddress& ip);
// Identify a shared network address like "100.72.16.122"
// (see RFC6598)
bool IPIsSharedNetwork(const IPAddress& ip);
// Identify if an IP is "private", that is a loopback
// or an address belonging to a link-local, a private network or a shared
// network.
bool IPIsPrivate(const IPAddress& ip);
bool IPIsUnspec(const IPAddress& ip);
size_t HashIP(const IPAddress& ip);

// These are only really applicable for IPv6 addresses.
bool IPIs6Bone(const IPAddress& ip);
bool IPIs6To4(const IPAddress& ip);
bool IPIsMacBased(const IPAddress& ip);
bool IPIsSiteLocal(const IPAddress& ip);
bool IPIsTeredo(const IPAddress& ip);
bool IPIsULA(const IPAddress& ip);
bool IPIsV4Compatibility(const IPAddress& ip);
bool IPIsV4Mapped(const IPAddress& ip);

// Returns the precedence value for this IP as given in RFC3484.
int IPAddressPrecedence(const IPAddress& ip);

// Returns 'ip' truncated to be 'length' bits long.
IPAddress TruncateIP(const IPAddress& ip, int length);

IPAddress GetLoopbackIP(int family);
IPAddress GetAnyIP(int family);

// Returns the number of contiguously set bits, counting from the MSB in network
// byte order, in this IPAddress. Bits after the first 0 encountered are not
// counted.
int CountIPMaskBits(const IPAddress& mask);

}  // namespace evpp


