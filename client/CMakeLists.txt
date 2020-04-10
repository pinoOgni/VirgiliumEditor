cmake_minimum_required(VERSION 3.15)
project(VirgiliumClient)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
Set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(Qt5Network REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)





add_executable(VirgiliumClient main.cpp virgilium_client.cpp virgilium_client.h server_virgilium.cpp server_virgilium.h constants.h message.cpp message.h symbol.cpp symbol.h exceptions.cpp exceptions.h)

target_link_libraries(VirgiliumClient Qt5::Network)
target_link_libraries(VirgiliumClient Qt5::Core  )
target_link_libraries(VirgiliumClient Qt5::Widgets  )