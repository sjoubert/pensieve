find_package(Boost COMPONENTS program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

find_package(Microhttpd REQUIRED)
include_directories(${MICROHTTPD_INCLUDE_DIRS})

file(GLOB source_files "*.?pp")
add_executable(pensieved ${source_files})
target_link_libraries(pensieved ${MICROHTTPD_LIBRARIES} ${Boost_LIBRARIES} pensieve)
install(TARGETS pensieved DESTINATION bin)
