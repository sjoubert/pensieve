# Use Qt4 instead of Qt5 since QSystemTrayIcon is currently broken there
# May be fixed in 5.4.2 and/or 5.5
find_package(Qt4 COMPONENTS QtGui REQUIRED)
mark_as_advanced(QT_QMAKE_EXECUTABLE)

file(GLOB_RECURSE source_files "*.?pp" "*.qrc")
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_executable(qpensieve ${source_files})
target_link_libraries(qpensieve Qt4::QtGui pensieve)
