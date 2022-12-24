#include "open62541cpp/status_code.hpp"

#include <open62541/types.h>

namespace opc::ua {

std::string status_code::as_string() const {
  return UA_StatusCode_name(code_);
}
bool status_code::is_bad() const noexcept {
  return UA_StatusCode_isBad(code_);
}

}
