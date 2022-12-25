#pragma once

#include <chrono>
#include <functional>
#include <memory>
#include <utility>
#include <vector>

#include <open62541cpp/server_config.hpp>

struct UA_Server;

namespace opc::ua {

class status_code;
class timer;

class server {
public:
  server();
  explicit server(server_config &&);

  template <typename T> [[nodiscard]] T const &get() const noexcept {
    using value_t = std::remove_cvref_t<T>;
    if constexpr (std::is_same_v<value_t, server_config>) {
      return config_;
    } else {
      []<bool flag = false>() { static_assert(flag, "no match"); }
      ();
    }
  }

  [[nodiscard]] auto const &c_server() const noexcept { return server_; }

  status_code run();
  void change_run(bool new_run_state) {
    running_ = std::make_unique<bool>(new_run_state);
  }
  status_code run_startup();
  std::chrono::milliseconds run_iterate(bool wait_internal);
  status_code run_shutdown();

  [[nodiscard]] timer new_timer() const;
  std::shared_ptr<timer> new_discardable_timer();

private:
  std::unique_ptr<UA_Server, std::function<void(UA_Server *)>> server_{nullptr};
  server_config config_{};
  std::unique_ptr<bool> running_{std::make_unique<bool>(false)};
  std::vector<std::shared_ptr<timer>> timer_refs_{};
};

} // namespace opc::ua
