#include <open62541/server.h>
#include <open62541cpp/server.hpp>
#include <open62541cpp/timer.hpp>

namespace opc::ua {

timer::timer(server const *owner) : owner_(owner) {}
timer::~timer() noexcept { stop(); }
status_code timer::start() {
  if (owner_ == nullptr) {
    return status_code(UA_STATUSCODE_BADUNEXPECTEDERROR);
  }

  constexpr auto c_callback{[](UA_Server *server, void *data) -> void {
    decltype(auto) self{static_cast<decltype(this)>(data)};
    if (self == nullptr) {
      throw std::runtime_error("open62541 timer callback error");
    }
    if (self->cb_) {
      try {
        std::invoke(self->cb_.value(), *self);
      } catch (const std::exception &ex) {
        self->stop(); // redundant?
        throw;
      }
    }
    self->stop(); // redundant?
  }};

  decltype(UA_DateTime_nowMonotonic()) int_time_point{};
  if (timeout_) {
    int_time_point = UA_DateTime_nowMonotonic() +
                     static_cast<decltype(UA_DateTime_nowMonotonic())>(
                         timeout_->count() / 100);
  } else if (time_point_) {
    const auto diff{time_point_.value() -
                    decltype(time_point_)::value_type::clock::now()};
    int_time_point =
        UA_DateTime_nowMonotonic() +
        static_cast<decltype(UA_DateTime_nowMonotonic())>(diff.count() / 100);
  }
  const auto code{status_code{UA_Server_addTimedCallback(
      owner_->c_server().get(), c_callback, static_cast<void *>(this),
      int_time_point, &this->id_)}};
  if (!code.is_bad()) {
    state_ = state::waiting;
  }
  return code;
}
void timer::stop() {
  state_ = state::done;
  if (owner_ == nullptr) {
    // Todo log
    return;
  }
  UA_Server_removeCallback(owner_->c_server().get(), id_);
}

} // namespace opc::ua
