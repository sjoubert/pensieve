cmake_minimum_required(VERSION 3.0)

project(pensieve LANGUAGES CXX VERSION 0.6)

# Update CMake module path
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Build configuration
if(CMAKE_BUILD_TYPE STREQUAL "")
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
endif()
set(CMAKE_CXX_FLAGS "-std=c++14 -Wall -Wextra ${CMAKE_CXX_FLAGS}")

add_subdirectory(source)
include(psvPackaging)
