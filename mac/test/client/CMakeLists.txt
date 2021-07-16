cmake_minimum_required(VERSION 3.14)
project(client)

include_directories(../../chatkit)

add_executable(client main.cpp)

target_link_libraries(client)