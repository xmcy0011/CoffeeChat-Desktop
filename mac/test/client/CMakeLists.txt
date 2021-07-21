cmake_minimum_required(VERSION 3.14)
project(client)

find_package(protobuf CONFIG REQUIRED)

add_executable(simple_client simple_client.cpp)
# 不用include chatKit目录，因为chatKit目录的CMakeLists.txt中
# 通过target_include_directories 可被外部感知
target_link_libraries(simple_client cim::chatkit protobuf::libprotobuf-lite)