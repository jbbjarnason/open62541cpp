#include <string>

#include "open62541cpp/open62541cpp.hpp"

#include <fmt/core.h>

exported_class::exported_class()
    : m_name {fmt::format("{}", "open62541cpp")}
{
}

auto exported_class::name() const -> const char*
{
  return m_name.c_str();
}
