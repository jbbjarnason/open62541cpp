#pragma once

#include <cstdint>
#include <string>

namespace opc::ua {

class status_code {
public:
  status_code() = default;
  explicit status_code(std::uint32_t&& code) : code_(code) {}
  [[nodiscard]] auto const& value() const noexcept {
    return code_;
  }
  [[nodiscard]] std::string as_string() const;
  [[nodiscard]] bool is_bad() const noexcept;
private:
  std::uint32_t code_;
};

}