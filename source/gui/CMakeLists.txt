find_package(Boost COMPONENTS program_options REQUIRED)

if(PSV_USE_QT4)
  find_package(Qt4 COMPONENTS QtGui QtNetwork REQUIRED)
  mark_as_advanced(QT_QMAKE_EXECUTABLE)
  set(qtLibrary Qt4::QtGui Qt4::QtNetwork)
  set(Qt_VERSION ${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}.${QT_VERSION_PATCH})
else()
  find_package(Qt5 COMPONENTS Widgets Network REQUIRED)
  mark_as_advanced(Qt5_DIR Qt5Core_DIR Qt5Gui_DIR Qt5Widgets_DIR Qt5Network_DIR)
  set(qtLibrary Qt5::Widgets Qt5::Network)
  set(Qt_VERSION ${Qt5_VERSION})
endif()

add_executable(qpensieve
  main.cpp
  psvLogDialog.cpp
  psvLogDialog.hpp
  psvLogDialog.ui
  psvPensieveWidget.cpp
  psvPensieveWidget.hpp
  psvPensieveWidget.ui
  psvPensieveWindow.cpp
  psvPensieveWindow.hpp
  psvPensieveWindow.ui
  psvSettingsDialog.cpp
  psvSettingsDialog.hpp
  psvSettingsDialog.ui
  psvThoughtWidget.cpp
  psvThoughtWidget.hpp
  psvThoughtWidget.ui
  res/resources.qrc
)
set_target_properties(qpensieve PROPERTIES AUTOMOC ON AUTOUIC ON AUTORCC ON)
# The Q_OBJECT macro comes with virtual methods not marked 'override'
# With Clang >= 3.6 -Wno-inconsistent-missing-override is triggered when
# some virtual methods of a class are marked 'override' while others are not
# This should be fixed in Qt 5.5, commit 3092bd56c394f22e027be59368789546afb5ca98
if(${Qt_VERSION} VERSION_LESS 5.5 AND "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
  target_compile_options(qpensieve PRIVATE "-Wno-inconsistent-missing-override")
endif()
target_include_directories(qpensieve PRIVATE ${CMAKE_CURRENT_BINARY_DIR} ${Boost_INCLUDE_DIRS})
target_link_libraries(qpensieve PRIVATE ${qtLibrary} ${Boost_LIBRARIES} pensieve)
install(TARGETS qpensieve DESTINATION bin)
