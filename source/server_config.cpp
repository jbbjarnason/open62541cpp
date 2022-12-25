#include "open62541cpp/server_config.hpp"
#include <open62541/server.h>

// Config is maintained by C library, so we should not double erase it
inline constexpr auto dont_destroy_config{[](UA_ServerConfig *) -> void {}};

namespace opc::ua {
server_config::server_config(UA_ServerConfig *config)
    : config_(config, dont_destroy_config) {}
} // namespace opc::ua
