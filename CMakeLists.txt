cmake_minimum_required(VERSION 3.5)

project(pensieve LANGUAGES CXX VERSION 0.10)

# Update CMake module path
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Build configuration
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS FALSE)
if(CMAKE_CXX_COMPILER_ID MATCHES "(GNU|Clang)")
  set(CMAKE_CXX_FLAGS "-Wall -Wextra ${CMAKE_CXX_FLAGS}")
endif()

add_subdirectory(source)
include(psvPackaging)
