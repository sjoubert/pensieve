# By default, use Qt4 instead of Qt5 since QSystemTrayIcon is currently broken there
# May be fixed in 5.4.2 and/or 5.5
# See https://bugreports.qt.io/browse/QTBUG-32811
if(PSV_USE_QT5)
  find_package(Qt5 COMPONENTS Widgets REQUIRED)
  mark_as_advanced(Qt5_DIR Qt5Core_DIR Qt5Gui_DIR Qt5Widgets_DIR)
  set(qtLibrary Qt5::Widgets)
else()
  find_package(Qt4 COMPONENTS QtGui REQUIRED)
  mark_as_advanced(QT_QMAKE_EXECUTABLE)
  set(qtLibrary Qt4::QtGui)
endif()

file(GLOB_RECURSE source_files "*.?pp" "*.qrc")
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_executable(qpensieve ${source_files})
target_link_libraries(qpensieve ${qtLibrary} pensieve)
