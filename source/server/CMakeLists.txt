find_package(Microhttpd REQUIRED)
include_directories(${MICROHTTPD_INCLUDE_DIRS})

file(GLOB source_files "*.?pp")
add_executable(pensieved ${source_files})
target_link_libraries(pensieved ${MICROHTTPD_LIBRARIES})
