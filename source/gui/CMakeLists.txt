find_package(Qt5 COMPONENTS Widgets REQUIRED)
mark_as_advanced(Qt5_DIR Qt5Core_DIR Qt5Gui_DIR Qt5Widgets_DIR)

file(GLOB_RECURSE source_files "*.?pp" "*.qrc")
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_executable(qpensieve ${source_files})
target_link_libraries(qpensieve Qt5::Widgets)
