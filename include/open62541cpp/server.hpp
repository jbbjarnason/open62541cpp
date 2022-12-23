#pragma once

#include <functional>
#include <memory>

struct UA_Server;

namespace opc::ua {

class server_config;

class server {
public:
  server();
  explicit server(server_config&&);

private:
  std::unique_ptr<UA_Server, std::function<void(UA_Server*)>> server_;
};

}
