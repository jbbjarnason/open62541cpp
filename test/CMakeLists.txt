cmake_minimum_required(VERSION 3.14)

project(open62541cppTests LANGUAGES CXX)

include(../cmake/project-is-top-level.cmake)
include(../cmake/folders.cmake)

# ---- Dependencies ----

if(PROJECT_IS_TOP_LEVEL)
  find_package(open62541cpp REQUIRED)
  enable_testing()
endif()

include(FetchContent)
FetchContent_Declare(
  ut
  GIT_REPOSITORY https://github.com/boost-ext/ut.git
  GIT_TAG        master
)
FetchContent_MakeAvailable(ut)

# ---- Tests ----

add_executable(open62541cpp_test source/coro_test.cpp)
target_link_libraries(open62541cpp_test
PRIVATE
  open62541cpp::open62541cpp
  Boost::ut
)
target_compile_features(open62541cpp_test PRIVATE cxx_std_20)

# ---- End-of-file commands ----

add_folders(Test)
