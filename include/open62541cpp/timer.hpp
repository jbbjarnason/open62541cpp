#pragma once

#include <chrono>
#include <optional>
#include <type_traits>
#include <functional>

#include <open62541cpp/status_code.hpp>

namespace opc::ua {

struct server;

class timer {
public:
  explicit timer(server const *owner);
  /// \brief timer is stopped on destruction
  ~timer() noexcept;
  using callback_t = std::function<void(timer &)>;
  timer &set(auto &&value) {
    using value_t = std::remove_reference_t<decltype(value)>;
    if constexpr (std::is_same_v<value_t, ns>) {
      timeout_ = std::forward<>(value);
    } else if constexpr (std::is_same_v<value_t,
                                        std::chrono::time_point<clock, us>>) {
      time_point_ = std::forward<>(value);
    } else if constexpr (std::is_same_v<value_t, callback_t>) {
      cb_ = std::forward<>(value);
    } else {
      []<bool flag = false>() { static_assert(flag, "no setter match"); }
      ();
    }
    return *this;
  }
  status_code start();
  void stop();

  enum struct state : std::uint8_t {
    undefined,
    waiting,
    done,
  };
  using callback_id = std::uint64_t;
  using ns = std::chrono::nanoseconds;
  using us = std::chrono::microseconds;
  using clock = std::chrono::high_resolution_clock;

  template <typename T> T const &get() const noexcept {
    if constexpr (std::is_same_v<T, decltype(timeout_)>) {
      return timeout_;
    } else if constexpr (std::is_same_v<T, decltype(time_point_)>) {
      return time_point_;
    } else if constexpr (std::is_same_v<T, decltype(cb_)>) {
      return cb_;
    } else if constexpr (std::is_same_v<T, decltype(id_)>) {
      return id_;
    } else if constexpr (std::is_same_v<T, decltype(state_)>) {
      return state_;
    } else {
      []<bool flag = false>() { static_assert(flag, "no getter match"); }
      ();
    }
  }

private:
  server const *owner_{nullptr};
  std::optional<ns> timeout_{};
  std::optional<std::chrono::time_point<clock, us>> time_point_{};
  std::optional<callback_t> cb_{};
  callback_id id_{};
  state state_{state::undefined};
};

} // namespace opc::ua