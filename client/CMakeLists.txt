cmake_minimum_required(VERSION 3.15)
project(provaQt2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)

set(CMAKE_AUTORCC ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Charts REQUIRED)
find_package(Qt5Network REQUIRED)



list(APPEND LIBRARIES
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Charts
        Qt5::Network)
list(REMOVE_DUPLICATES LIBRARIES)

qt5_add_resources(PROJECT_RESOURCE Resources/Icons.qrc)

add_executable(provaQt2 ${PROJECT_RESOURCE} main.cpp  mainwindow.ui  mainwindow.cpp mainwindow.h provaQt2_autogen/include/ui_mainwindow.h virgilium_client.cpp virgilium_client.h server_virgilium.cpp server_virgilium.h constants.h message.cpp message.h symbol.cpp symbol.h exceptions.cpp exceptions.h)
target_link_libraries(provaQt2 ${LIBRARIES})