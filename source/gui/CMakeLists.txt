find_package(Boost COMPONENTS program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

# By default, use Qt4 instead of Qt5 since QSystemTrayIcon is currently broken there
# May be fixed in 5.4.2 and/or 5.5
# See https://bugreports.qt.io/browse/QTBUG-32811
if(PSV_USE_QT5)
  find_package(Qt5 COMPONENTS Widgets Network REQUIRED)
  mark_as_advanced(Qt5_DIR Qt5Core_DIR Qt5Gui_DIR Qt5Widgets_DIR Qt5Network_DIR)
  set(qtLibrary Qt5::Widgets Qt5::Network)
else()
  find_package(Qt4 COMPONENTS QtGui QtNetwork REQUIRED)
  mark_as_advanced(QT_QMAKE_EXECUTABLE)
  set(qtLibrary Qt4::QtGui Qt4::QtNetwork)
endif()

file(GLOB_RECURSE source_files "*.?pp" "*.ui" "*.qrc")
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(qpensieve ${source_files})
target_link_libraries(qpensieve ${qtLibrary} ${Boost_LIBRARIES} pensieve)
