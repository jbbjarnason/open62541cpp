#include "open62541cpp/server.hpp"
#include "open62541cpp/server_config.hpp"
#include "open62541cpp/status_code.hpp"

#include <open62541/server.h>
#include <open62541/server_config_default.h>

inline constexpr auto destroy_server{
    [](UA_Server *server) -> void { UA_Server_delete(server); }};

// Config is maintained by C library, so we should not double erase it
inline constexpr auto dont_destroy_config{[](UA_ServerConfig *) -> void {}};

namespace opc::ua {

server::server()
    : server_(UA_Server_new(), destroy_server),
      config_(
          static_cast<server_config *>(UA_Server_getConfig(server_.get()))) {}

server::server(server_config &&server_conf)
    : server_(
          UA_Server_newWithConfig(static_cast<UA_ServerConfig *>(&server_conf)),
          destroy_server),
      config_(
          static_cast<server_config *>(UA_Server_getConfig(server_.get()))) {}

status_code server::run() {
  running_ = std::make_unique<bool>(true);
  return status_code{UA_Server_run(server_.get(), running_.get())};
}
status_code server::run_startup() {
  return status_code{UA_Server_run_startup(server_.get())};
}
std::chrono::milliseconds server::run_iterate(bool wait_internal) {
  return std::chrono::milliseconds(UA_Server_run_iterate(server_.get(), wait_internal));
}
status_code server::run_shutdown() { return status_code{UA_Server_run_shutdown(server_.get())}; }

inline constexpr auto private_timed_callback(UA_Server* c_server) {
  constexpr auto c_callback{[](UA_Server *server, void *data) -> void {

  }};
}

status_code server::add_timed_callback(std::invocable<void, server&> auto cb) {

//  return status_code{UA_Server_addTimedCallback(server_.get(), c_callback, this, )};
  return status_code{};
}

} // namespace opc::ua