#pragma once

#include <string_view>
#include <open62541/server.h>

namespace opc::ua {

class server_config : public UA_ServerConfig {
public:
  server_config() = default;

};

}