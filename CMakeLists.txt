project(subway-panel)
cmake_minimum_required(VERSION 2.8)
find_package(Qt4 REQUIRED)
find_package(X11 REQUIRED)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

set(subway-panel_SRCS panelapplication.cpp applet.cpp applets/window-list/instanceofwindow.cpp subway-panel.cpp main.cpp applets/window-list/main.cpp)
qt4_automoc(${subway-panel_SRCS})
add_executable(subway-panel ${subway-panel_SRCS})
target_link_libraries(subway-panel ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${QT_QTDBUS_LIB_DEPENDENCIES} subway)
install(TARGETS subway-panel RUNTIME DESTINATION bin)
