include(CMakeDependentOption)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/psvConfigure.hpp.in ${CMAKE_CURRENT_BINARY_DIR}/psvConfigure.hpp @ONLY)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

option(PSV_BUILD_LIBRARY "Build pensieve library" ON)
if(PSV_BUILD_LIBRARY)
  message(STATUS "Configuring library")
  add_subdirectory(library)
  include_directories(library)
endif()

cmake_dependent_option(PSV_BUILD_SERVER "Build pensieve server" ON "PSV_BUILD_LIBRARY" OFF)
if(PSV_BUILD_SERVER)
  message(STATUS "Configuring server")
  add_subdirectory(server)
endif()

cmake_dependent_option(PSV_BUILD_GUI "Build pensieve graphical interface" ON "PSV_BUILD_LIBRARY" OFF)
# By default, use Qt4 instead of Qt5 since QSystemTrayIcon is currently broken there
# May be fixed in 5.4.2 and/or 5.5
# See https://bugreports.qt.io/browse/QTBUG-32811
cmake_dependent_option(PSV_USE_QT5 "Use Qt5 to build the graphical interface (default is Qt4)" OFF "PSV_BUILD_GUI" OFF)
if(PSV_BUILD_GUI)
  message(STATUS "Configuring graphical interface")
  add_subdirectory(gui)
endif()
