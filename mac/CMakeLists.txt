cmake_minimum_required(VERSION 3.14)

project(mac)

add_definitions(
        -g
        -fPIC
        -D__STDC_FORMAT_MACROS
        -Wall
        -Wno-unused-parameter
        -Wno-unused-function
        -Wno-unused-const-variable
        -Wno-unused-variable
        -Wno-unused-value
        -Wshadow
        -Wcast-qual
        -Wcast-align
        -Wwrite-strings
        -Wsign-compare
        -Winvalid-pch
        -fms-extensions
        -Wfloat-equal
        -Wextra
        -std=c++11
        #-Werror # 警告视为错误，比较严格，可以根据需求启用
)

# 库
add_subdirectory(chatkit)
add_subdirectory(hellochat)

# 生成demo
set(BUILD_DEMO true)
if (BUILD_DEMO)
    add_subdirectory(test/client)
    add_subdirectory(test/libuvdemo)
endif ()