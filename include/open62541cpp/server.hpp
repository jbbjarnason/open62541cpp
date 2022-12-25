#pragma once

#include "server_config.hpp"
#include <chrono>
#include <functional>
#include <memory>
#include <utility>
#include <vector>

struct UA_Server;

namespace opc::ua {

class server_config;
class status_code;

class server {
public:
  server();
  explicit server(server_config &&);

  [[nodiscard]] auto const& get_config() const noexcept {
    return config_;
  }

  status_code run();
  void change_run(bool new_run_state) {
    running_ = std::make_unique<bool>(new_run_state);
  }
  status_code run_startup();
  std::chrono::milliseconds run_iterate(bool wait_internal);
  status_code run_shutdown();

  // add future callback after nanoseconds
  using timer_cb_t = std::function<void()>;
  status_code add_timed_callback(timer_cb_t cb, std::chrono::nanoseconds const&);

private:
  using callback_id = std::uint64_t;
  struct callback_context{
    callback_id id{};
    timer_cb_t cb{nullptr};
    server* self;
  };

  std::unique_ptr<UA_Server, std::function<void(UA_Server *)>> server_{nullptr};
  server_config config_{};
  std::unique_ptr<bool> running_{std::make_unique<bool>(false)};
  std::vector<callback_context> timed_callback_mem_{};
};

} // namespace opc::ua
