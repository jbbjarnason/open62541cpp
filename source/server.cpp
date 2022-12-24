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
  return std::chrono::milliseconds(
      UA_Server_run_iterate(server_.get(), wait_internal));
}
status_code server::run_shutdown() {
  return status_code{UA_Server_run_shutdown(server_.get())};
}

status_code server::add_timed_callback(timer_cb_t cb, std::chrono::nanoseconds const& ns) {
  constexpr auto c_callback{[](UA_Server *server, void *data) -> void {
    decltype(auto) ctx{static_cast<callback_context *>(data)};
    if (ctx == nullptr || ctx->self == nullptr) {
      throw std::runtime_error("open62541 timer callback error");
    }
    try {
      std::invoke(ctx->cb);
    } catch (const std::exception &ex) {
      auto it [[maybe_unused]]{std::remove_if(
          std::begin(ctx->self->timed_callback_mem_),
          std::end(ctx->self->timed_callback_mem_),
          [&ctx](const auto &it) -> bool { return it.id == ctx->id; })};
      UA_Server_removeCallback(ctx->self->server_.get(), ctx->id); // redundant?
      throw;
    }
    auto it [[maybe_unused]]{std::remove_if(
        std::begin(ctx->self->timed_callback_mem_),
        std::end(ctx->self->timed_callback_mem_),
        [&ctx](const auto &it) -> bool { return it.id == ctx->id; })};
    UA_Server_removeCallback(ctx->self->server_.get(), ctx->id); // redundant?
  }};

  callback_context ctx{.id = {}, .cb = std::move(cb), .self = this};
  const auto future{
      UA_DateTime_nowMonotonic() +
      static_cast<decltype(UA_DateTime_nowMonotonic())>(ns.count() / 100)};
  const auto code{status_code{UA_Server_addTimedCallback(
      server_.get(), c_callback, &ctx, future, &ctx.id)}};
  if (code.is_bad()) {
    return code;
  }
  timed_callback_mem_.emplace_back(std::move(ctx));
  return code;
}

} // namespace opc::ua