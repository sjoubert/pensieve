#
# Find libjsoncpp library
#
# This module defines the following variables
#   JSONCPP_FOUND
#   JSONCPP_VERSION
#   JSONCPP_LIBRARIES
#   JSONCPP_INCLUDE_DIRS
#

find_package(PkgConfig QUIET)
if(PKGCONFIG_FOUND)
  pkg_check_modules(PC_JSONCPP QUIET jsoncpp)
endif()

find_path(JSONCPP_INCLUDE_DIR json/json.h
  HINTS ${PC_JSONCPP_INCLUDE_DIRS})

find_library(JSONCPP_LIBRARY jsoncpp
  HINTS ${PC_JSONCPP_LIBRARY_DIRS})

set(JSONCPP_LIBRARIES ${JSONCPP_LIBRARY})
set(JSONCPP_INCLUDE_DIRS ${JSONCPP_INCLUDE_DIR})
set(JSONCPP_VERSION ${PC_JSONCPP_VERSION})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Jsoncpp
  REQUIRED_VARS JSONCPP_LIBRARY JSONCPP_INCLUDE_DIR
  VERSION_VAR JSONCPP_VERSION)

mark_as_advanced(JSONCPP_INCLUDE_DIR JSONCPP_LIBRARY)
