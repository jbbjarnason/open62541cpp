#pragma once

#include <memory>
#include <functional>

struct UA_ServerConfig;

namespace opc::ua {

class server_config {
public:
  server_config() = default;
  explicit server_config(UA_ServerConfig *config);

  auto const &get() { return config_; }

private:
  std::unique_ptr<UA_ServerConfig, std::function<void(UA_ServerConfig *)>>
      config_;
};

} // namespace opc::ua