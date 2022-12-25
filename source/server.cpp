#include <algorithm>

#include "open62541cpp/server.hpp"
#include "open62541cpp/server_config.hpp"
#include "open62541cpp/status_code.hpp"
#include "open62541cpp/timer.hpp"

#include <open62541/server.h>
#include <open62541/server_config_default.h>

inline constexpr auto destroy_server{
    [](UA_Server *server) -> void { UA_Server_delete(server); }};

namespace opc::ua {

server::server()
    : server_(UA_Server_new(), destroy_server),
      config_(UA_Server_getConfig(server_.get())) {}

server::server(server_config &&server_conf)
    : server_(UA_Server_newWithConfig(server_conf.get().get()), destroy_server),
      config_(std::move(server_conf)) {}

status_code server::run() {
  running_ = std::make_unique<bool>(true);
  return status_code{UA_Server_run(server_.get(), running_.get())};
}
status_code server::run_startup() {
  return status_code{UA_Server_run_startup(server_.get())};
}
std::chrono::milliseconds server::run_iterate(bool wait_internal) {
  return std::chrono::milliseconds(
      UA_Server_run_iterate(server_.get(), wait_internal));
}
status_code server::run_shutdown() {
  return status_code{UA_Server_run_shutdown(server_.get())};
}
timer server::new_timer() const { return timer{this}; }
std::shared_ptr<timer> server::new_discardable_timer() {
  // Let's remove all timers which are not waiting from memory
  auto const forward_it [[maybe_unused]]{
      std::remove_if(std::begin(timer_refs_), std::end(timer_refs_),
                     [](const auto &timer_item) -> bool {
                       return timer_item->template get<timer::state>() !=
                              timer::state::active;
                     })};
  return timer_refs_.emplace_back(std::make_shared<timer>(this));
}

} // namespace opc::ua