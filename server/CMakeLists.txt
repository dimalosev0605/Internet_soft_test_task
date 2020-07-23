cmake_minimum_required(VERSION 3.5)

project(server LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5Core)
find_package(Qt5 COMPONENTS Network REQUIRED)

add_executable(server
  main.cpp
  server.h
  server.cpp
  thread_connections.h
  thread_connections.cpp
  connection.h
  connection.cpp
)
target_link_libraries(server Qt5::Core Qt5::Network)
