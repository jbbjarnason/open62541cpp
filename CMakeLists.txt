cmake_minimum_required(VERSION 3.14)

include(cmake/prelude.cmake)

project(
    open62541cpp
    VERSION 0.1.0
    DESCRIPTION "Open62541 C++20 Wrapper"
    HOMEPAGE_URL "https://www.open62541.org/"
    LANGUAGES CXX
)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

# ---- Declare library ----

add_library(
    open62541cpp_open62541cpp
    source/open62541cpp.cpp
)
add_library(open62541cpp::open62541cpp ALIAS open62541cpp_open62541cpp)

include(GenerateExportHeader)
generate_export_header(
    open62541cpp_open62541cpp
    BASE_NAME open62541cpp
    EXPORT_FILE_NAME export/open62541cpp/open62541cpp_export.hpp
    CUSTOM_CONTENT_FROM_VARIABLE pragma_suppress_c4251
)

if(NOT BUILD_SHARED_LIBS)
  target_compile_definitions(open62541cpp_open62541cpp PUBLIC OPEN62541CPP_STATIC_DEFINE)
endif()

set_target_properties(
    open62541cpp_open62541cpp PROPERTIES
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN YES
    VERSION "${PROJECT_VERSION}"
    SOVERSION "${PROJECT_VERSION_MAJOR}"
    EXPORT_NAME open62541cpp
    OUTPUT_NAME open62541cpp
)

target_include_directories(
    open62541cpp_open62541cpp ${warning_guard}
    PUBLIC
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)

target_include_directories(
    open62541cpp_open62541cpp SYSTEM
    PUBLIC
    "$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/export>"
)

target_compile_features(open62541cpp_open62541cpp PUBLIC cxx_std_20)

find_package(fmt REQUIRED)
target_link_libraries(open62541cpp_open62541cpp PRIVATE fmt::fmt)

# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

# ---- Examples ----

if(PROJECT_IS_TOP_LEVEL)
  option(BUILD_EXAMPLES "Build examples tree." "${open62541cpp_DEVELOPER_MODE}")
  if(BUILD_EXAMPLES)
    add_subdirectory(example)
  endif()
endif()

# ---- Developer mode ----

if(NOT open62541cpp_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
      AUTHOR_WARNING
      "Developer mode is intended for developers of open62541cpp"
  )
endif()

include(cmake/dev-mode.cmake)