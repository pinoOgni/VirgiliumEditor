cmake_minimum_required(VERSION 3.15)
project(VirgiliumServer)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
Set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(Qt5Network REQUIRED)
find_package(Qt5Core REQUIRED)

add_executable(VirgiliumServer main.cpp constants.h virgilium_server.cpp virgilium_server.h virgilium_client.cpp virgilium_client.h message.cpp message.h symbol.cpp symbol.h)

target_link_libraries(VirgiliumServer Qt5::Network)
target_link_libraries(VirgiliumServer Qt5::Core  )