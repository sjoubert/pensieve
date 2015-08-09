cmake_minimum_required(VERSION 3.0)

project(pensieve LANGUAGES CXX VERSION 0.8)

# Update CMake module path
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Build configuration
if(CMAKE_CXX_COMPILER_ID MATCHES "(GNU|Clang)")
  set(CMAKE_CXX_FLAGS "-std=c++14 -Wall -Wextra ${CMAKE_CXX_FLAGS}")
endif()

add_subdirectory(source)
include(psvPackaging)
