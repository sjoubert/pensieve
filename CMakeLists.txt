cmake_minimum_required(VERSION 3.0)

project(pensieve LANGUAGES CXX VERSION 0.0)

# Build configuration
if(CMAKE_BUILD_TYPE STREQUAL "")
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
endif()
set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
option(BUILD_SHARED_LIBS "Build shared libraries instead of static ones" ON)

add_subdirectory(source)
