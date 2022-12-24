#pragma once

#include <coroutine>

template<typename self_t, typename data_t>
inline constexpr auto private_timed_callback(self_t* c_server) {
  constexpr auto c_callback{[](self_t *server, void *data) -> void {

  }};
}