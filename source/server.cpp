#include "open62541cpp/server.hpp"
#include "open62541cpp/server_config.hpp"

#include <open62541/server.h>
#include <open62541/server_config_default.h>

inline constexpr auto destroy_server{
    [](UA_Server *server) -> void { UA_Server_delete(server); }};

namespace opc::ua {

server::server() : server_(UA_Server_new(), destroy_server) {}

server::server(server_config &&server_conf)
    : server_(
          UA_Server_newWithConfig(static_cast<UA_ServerConfig *>(&server_conf)),
          destroy_server) {}

} // namespace opc::ua