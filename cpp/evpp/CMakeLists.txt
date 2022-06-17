set(base_SRCS
  buffer.cc
  connector.cc
  dns_resolver.cc
  duration.cc
  event_loop.cc
  event_loop_thread.cc
  event_loop_thread_pool.cc
  event_watcher.cc
  fd_channel.cc
  inner_pre.cc
  invoke_timer.cc
  libevent.cc
  listener.cc
  socket_address.cc
  sockets.cc
  tcp_client.cc
  tcp_conn.cc
  tcp_server.cc
)

set(evpphttp_SRCS
  evpphttp/http_parser_cpp.cc
  evpphttp/http_request.cc
  evpphttp/http_response.cc
  evpphttp/service.cc
)
set(http_SRCS
  http/context.cc
  http/http_server.cc
  http/service.cc
)

set(httpc_SRCS
  httpc/conn.cc
  httpc/conn_pool.cc
  httpc/request.cc
  httpc/response.cc
  httpc/ssl.cc
  httpc/url_parser.cc
)

set(udp_SRCS
  udp/sync_udp_client.cc
  udp/udp_server.cc
)

add_library(evpp ${base_SRCS} ${evpphttp_SRCS} ${http_SRCS} ${httpc_SRCS} ${udp_SRCS})
add_dependencies(evpp boost brpc glog)  
message("PROJECT_SOURCE_DIR=${PROJECT_SOURCE_DIR}")
target_include_directories(evpp PUBLIC ${CMAKE_SOURCE_DIR} ${DEP_INC} ${PROJECT_SOURCE_DIR}/third-party)