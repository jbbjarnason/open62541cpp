#pragma once

#include <chrono>
#include <functional>
#include <optional>
#include <type_traits>

namespace opc::ua {

class server;
class status_code;

class timer {
public:
  enum struct type : std::uint8_t {
    single_shot,
    repeated,
  };
  enum struct state : std::uint8_t {
    undefined,
    active,
    done,
  };
  using callback_t = std::function<void(timer &)>;
  using callback_id = std::uint64_t;
  using interval_t = std::chrono::duration<double, std::milli>;
  using timeout_t = std::chrono::nanoseconds;
  using time_point_t = std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::milliseconds>;

  explicit timer(server const *owner);
  /// \brief timer is stopped on destruction
  ~timer() noexcept;

  timer &set(auto &&value) {
    using value_t = std::remove_reference_t<decltype(value)>;
    if constexpr (std::is_same_v<value_t, decltype(timeout_)::value_type>) {
      timeout_ = std::forward<>(value);
    } else if constexpr (std::is_same_v<value_t,
                                        decltype(time_point_)::value_type>) {
      time_point_ = std::forward<>(value);
    } else if constexpr (std::is_same_v<value_t,
                                        decltype(interval_)::value_type>) {
      interval_ = std::forward<>(value);
      if (state_ == state::active)
        changedInterval();
    } else if constexpr (std::is_same_v<value_t, decltype(cb_)>) {
      cb_ = std::forward<>(value);
    } else if constexpr (std::is_same_v<value_t, decltype(type_)>) {
      type_ = std::forward<>(value);
    } else {
      []<bool flag = false>() { static_assert(flag, "no setter match"); }
      ();
    }
    return *this;
  }

  status_code start();

  void stop();

  template <typename T> T const &get() const noexcept {
    if constexpr (std::is_same_v<T, decltype(timeout_)>) {
      return timeout_;
    } else if constexpr (std::is_same_v<T, decltype(time_point_)>) {
      return time_point_;
    } else if constexpr (std::is_same_v<T, decltype(interval_)>) {
      return interval_;
    } else if constexpr (std::is_same_v<T, decltype(cb_)>) {
      return cb_;
    } else if constexpr (std::is_same_v<T, decltype(id_)>) {
      return id_;
    } else if constexpr (std::is_same_v<T, decltype(state_)>) {
      return state_;
    } else if constexpr (std::is_same_v<T, decltype(type_)>) {
      return type_;
    } else {
      []<bool flag = false>() { static_assert(flag, "no getter match"); }
      ();
    }
  }
private:
  server const *owner_{nullptr};

  std::optional<interval_t> interval_{}; // For repeated timer
  std::optional<timeout_t> timeout_{};  // For single shot timer
  std::optional<time_point_t> time_point_{};

  std::optional<callback_t> cb_{};
  callback_id id_{};
  state state_{state::undefined};
  type type_{type::single_shot};

  void changedInterval();

};

} // namespace opc::ua