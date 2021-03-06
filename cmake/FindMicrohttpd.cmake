#
# Find libmicrohttpd library
#
# This module defines the following variables
#   MICROHTTPD_FOUND
#   MICROHTTPD_VERSION
#   MICROHTTPD_LIBRARIES
#   MICROHTTPD_INCLUDE_DIRS
#

find_package(PkgConfig QUIET)
if(PKGCONFIG_FOUND)
  pkg_check_modules(PC_MICROHTTPD QUIET libmicrohttpd)
endif()

find_path(MICROHTTPD_INCLUDE_DIR microhttpd.h
  HINTS ${PC_MICROHTTPD_INCLUDE_DIRS})

find_library(MICROHTTPD_LIBRARY microhttpd
  HINTS ${PC_MICROHTTPD_LIBRARY_DIRS})

set(MICROHTTPD_LIBRARIES ${MICROHTTPD_LIBRARY})
set(MICROHTTPD_INCLUDE_DIRS ${MICROHTTPD_INCLUDE_DIR})
set(MICROHTTPD_VERSION ${PC_MICROHTTPD_VERSION})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Microhttpd
  REQUIRED_VARS MICROHTTPD_LIBRARY MICROHTTPD_INCLUDE_DIR
  VERSION_VAR MICROHTTPD_VERSION)

mark_as_advanced(MICROHTTPD_INCLUDE_DIR MICROHTTPD_LIBRARY)
