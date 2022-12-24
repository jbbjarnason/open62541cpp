#include <atomic>
#include <boost/ut.hpp>

#include "coro.hpp"

struct a_type;
typedef void (*c_callback_t)(a_type *some_type, void *context);

struct a_type {
  bool value{false};
  void setCallback(c_callback_t cb) {
    cb_ = cb;
  }
  void callCallback(void* context) {
    if (cb_) cb_(this, context);
    else std::exit(1);
  }
private:
  c_callback_t cb_{nullptr};
};

//
//template<typename self_t>
//inline constexpr auto c_callback_wrapper(self_t* self) -> c_callback_t {
//  constexpr auto c_callback{[](self_t *server, void *data) -> void {
//
//  }};
//  return c_callback;
//}

int main() {

  using namespace boost::ut;

  "c_callback_coroutine"_test = [] {
    a_type object;
    constexpr auto cb{ [](a_type *some_type, void *context){
      some_type->value = true;
    } };
    object.setCallback(cb);
    object.callCallback(nullptr);
    expect(object.value);
  };

  return 0;
}
