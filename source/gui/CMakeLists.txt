find_package(Qt5 COMPONENTS Widgets REQUIRED)

file(GLOB source_files "*.?pp")
add_executable(qpensieve ${source_files})
target_link_libraries(qpensieve Qt5::Widgets)
