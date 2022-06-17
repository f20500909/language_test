/*
 *  Copyright 2004 The WebRTC Project Authors. All rights reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */
#pragma once

#include <boost/optional.hpp>
#include <boost/utility/string_view.hpp>
#include "evpp/sockets.h"

namespace evpp {

// Records an IP address and port.
class SocketAddress {
 public:
  // Creates a nil address.
  SocketAddress();

  SocketAddress(const struct sockaddr_in& in4);
  SocketAddress(const struct sockaddr_in6& in6);
  SocketAddress(const struct in_addr& in4, uint16_t port);
  SocketAddress(const struct in6_addr& in6, uint16_t port);
  // Creates the address with the given IP and port.
  // IP is given as an integer in host byte order. V4 only, to be deprecated.
  // DCHECKs that port is in valid range (0 to 2^16-1).
  SocketAddress(uint32_t ip_as_host_order_integer, uint16_t port);

  // Creates a copy of the given address.
  SocketAddress(const SocketAddress& addr);

  // Resets to the nil address.
  void Clear();

  // Determines if this is a nil address (empty hostname, any IP, null port)
  bool IsNil() const;

  // Replaces our address with the given one.
  SocketAddress& operator=(const SocketAddress& addr);

  // Changes the IP of this address to the given one, and clears the hostname
  // IP is given as an integer in host byte order. V4 only, to be deprecated..
  void SetIP(const struct in_addr& in4);
  void SetIP(const struct in6_addr& in6);
  void SetIP(uint32_t ip_as_host_order_integer);

  // Changes the port of this address to the given one.
  // DCHECKs that port is in valid range (0 to 2^16-1).
  void SetPort(uint16_t port);

  // Returns the IP address as a host byte order integer.
  // Returns 0 for non-v4 addresses.
  uint32_t ip() const;

  in_addr* ipv4_address() noexcept;
  in6_addr* ipv6_address() noexcept;
  const in_addr* ipv4_address() const noexcept;
  const in6_addr* ipv6_address() const noexcept;
  int length() const noexcept;

  int family() const noexcept { return ss_.ss_family; }

  // Returns the port part of this address.
  uint16_t port() const;

  // Returns the scope ID associated with this address. Scope IDs are a
  // necessary addition to IPv6 link-local addresses, with different network
  // interfaces having different scope-ids for their link-local addresses.
  // IPv4 address do not have scope_ids and sockaddr_in structures do not have
  // a field for them.
  uint32_t scope_id() const;

  void SetScopeID(uint32_t id);

  // Returns hostname:port or [hostname]:port.
  std::string ToString() const;

  std::string ToIPString() const;

  // Determines whether this represents a missing / any IP address.
  // That is, 0.0.0.0 or ::.
  // Hostname and/or port may be set.
  bool IsAnyIP() const;

  // Determines whether the IP address refers to a loopback address.
  // For v4 addresses this means the address is in the range 127.0.0.0/8.
  // For v6 addresses this means the address is ::1.
  bool IsLoopbackIP() const;

  // Determines whether the IP address is in one of the private ranges:
  // For v4: 127.0.0.0/8 10.0.0.0/8 192.168.0.0/16 172.16.0.0/12.
  // For v6: FE80::/16 and ::1.
  bool IsPrivateIP() const;

  // Determines whether the hostname has been resolved to an IP.
  bool IsUnresolvedIP() const;

  // Determines whether this address is identical to the given one.
  bool operator==(const SocketAddress& addr) const;
  inline bool operator!=(const SocketAddress& addr) const {
    return !this->operator==(addr);
  }

  // Compares based on IP and then port.
  bool operator<(const SocketAddress& addr) const;

  // Hashes this address into a small number.
  size_t Hash() const;

  struct sockaddr* SocketAddr() noexcept {
    return sock::sockaddr_cast(&ss_);
  }
  const struct sockaddr* SocketAddr() const noexcept {
    return sock::sockaddr_cast(&ss_);
  }

 public:
  mutable struct sockaddr_storage ss_;
};

boost::optional<SocketAddress> MakeSocketAddress(const sockaddr_storage& addr);
boost::optional<SocketAddress> MakeSocketAddress(const std::string& address);
SocketAddress EmptySocketAddressWithFamily(int family);

}  // namespace evpp
