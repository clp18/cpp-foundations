#Project Setup
cmake_minimum_required(VERSION 3.25)
project(cpp_foundations LANGUAGES CXX)

#Language Standards
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#Header-only library for now
add_library(cpp_foundations INTERFACE)
target_include_directories(cpp_foundations INTERFACE include)

#GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.tar.gz
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

#Tests
enable_testing()
add_executable(cpp_foundations_tests tests/test_math.cpp)
target_link_libraries(cpp_foundations_tests PRIVATE cpp_foundations GTest::gtest_main)
# -Wunsafe-buffer-usage is a Clang only flag
target_compile_options(cpp_foundations_tests PRIVATE -Wall -Wextra -Wpedantic -Wformat=2 -Wconversion -Wimplicit-fallthrough -Werror $<$<CXX_COMPILER_ID:Clang,AppleClang>:-Wunsafe-buffer-usage>)

include(GoogleTest)
gtest_discover_tests(cpp_foundations_tests)
