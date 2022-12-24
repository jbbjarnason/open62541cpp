#pragma once

#include <functional>
#include <memory>
#include <chrono>
#include <utility>

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

  status_code add_timed_callback(std::invocable<void, server&> auto cb);

private:
  std::unique_ptr<UA_Server, std::function<void(UA_Server *)>> server_{nullptr};
  std::unique_ptr<server_config, std::function<void(server_config *)>> config_{nullptr};
  std::unique_ptr<bool> running_{std::make_unique<bool>(false)};
};

} // namespace opc::ua
