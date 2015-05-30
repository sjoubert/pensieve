find_package(Boost COMPONENTS program_options REQUIRED)
find_package(Microhttpd REQUIRED)

add_executable(pensieved
  main.cpp
  psvServer.cpp
  psvServer.hpp
)
if(UNIX)
  target_compile_definitions(pensieved PRIVATE PSV_POSIX)
endif()
target_include_directories(pensieved PRIVATE ${MICROHTTPD_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})
target_link_libraries(pensieved PRIVATE ${MICROHTTPD_LIBRARIES} ${Boost_LIBRARIES} pensieve)
install(TARGETS pensieved DESTINATION bin)
