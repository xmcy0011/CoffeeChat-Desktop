project(libuv_tcp_client)

find_package(unofficial-libuv CONFIG REQUIRED)

add_executable(libuv_tcp_client tcp_client.h tcp_client.cpp client.cpp)

target_link_libraries(libuv_tcp_client unofficial::libuv::libuv pthread)

add_executable(libuv_tcp_server tcp_server.h tcp_server.cpp server.cpp)