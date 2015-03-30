find_package(Jsoncpp REQUIRED)
include_directories(${JSONCPP_INCLUDE_DIRS})

file(GLOB source_files "*.?pp")
add_library(pensieve ${source_files})
target_link_libraries(pensieve ${JSONCPP_LIBRARIES})
set_target_properties(pensieve PROPERTIES VERSION ${PROJECT_VERSION})
