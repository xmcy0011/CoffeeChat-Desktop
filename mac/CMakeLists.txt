cmake_minimum_required(VERSION 3.14)

project(mac VERSION 0.1 LANGUAGES CXX)

# 库
add_subdirectory(chatkit)
add_subdirectory(hellochat)

# 生成demo
#set(BUILD_DEMO true)
if(BUILD_DEMO)
    add_subdirectory(test/client)
endif()