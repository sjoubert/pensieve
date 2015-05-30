find_package(Jsoncpp REQUIRED)

add_library(pensieve STATIC
  psvPensieve.cpp
  psvPensieve.hpp
  psvThought.cpp
  psvThought.hpp
)
target_include_directories(pensieve PRIVATE ${JSONCPP_INCLUDE_DIRS})
target_link_libraries(pensieve PRIVATE ${JSONCPP_LIBRARIES})
