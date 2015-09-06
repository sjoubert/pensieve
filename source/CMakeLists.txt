include(CMakeDependentOption)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/psvConfigure.hpp.in ${CMAKE_CURRENT_BINARY_DIR}/psvConfigure.hpp @ONLY)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

option(PSV_BUILD_LIBRARY "Build pensieve library" ON)
if(PSV_BUILD_LIBRARY)
  message(STATUS "Configuring pensieve library")
  add_subdirectory(library)
  include_directories(library)
endif()

cmake_dependent_option(PSV_BUILD_SERVER "Build pensieve server" ON "PSV_BUILD_LIBRARY" OFF)
if(PSV_BUILD_SERVER)
  message(STATUS "Configuring pensieve server")
  add_subdirectory(server)
endif()

cmake_dependent_option(PSV_BUILD_GUI "Build pensieve graphical interface" ON "PSV_BUILD_LIBRARY" OFF)
if(PSV_BUILD_GUI)
  message(STATUS "Configuring pensieve graphical interface")
  add_subdirectory(gui)
endif()
