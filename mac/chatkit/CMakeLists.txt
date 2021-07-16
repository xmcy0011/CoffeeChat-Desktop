cmake_minimum_required(VERSION 3.14)

set(CHAT_KIT_LIB cimsdk)

project(${CHAT_KIT_LIB} VERSION 0.1 LANGUAGES CXX)

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
        #-Werror # 警告视为错误，比较严格，可以根据需求启用
        -std=c++14
)

file(GLOB CHAT_KIT_SRC
        cim/*/*.h cim/*/*.cpp cim/*/*.cc
        cim/base/util/*.*
        cim/base/file/*.*
        cim/*.h
        cim/*.cpp
        )

#find_package(protobuf REQUIRED)
#find_package(spdlog REQUIRED)
#find_package(SQLiteCpp REQUIRED)
#find_package(nlohmann_json REQUIRED)

if(APPLE)
    set(VCPKG_INCLUDE_DIR /Users/xuyc/repo/github/vcpkg/installed/x64-osx/include)
    set(VCPKG_LIB_DIR /Users/xuyc/repo/github/vcpkg/installed/x64-osx/lib)
endif()

include_directories(. ${VCPKG_INCLUDE_DIR})
link_directories(${VCPKG_LIB_DIR})

# 动态库
add_library(${CHAT_KIT_LIB} ${CHAT_KIT_SRC})
#add_library(${CHAT_KIT_LIB} STATIC)

target_link_libraries(${CHAT_KIT_LIB}
        # 静态库
        llibprotobuf-lite
        lspdlog
        lSQLiteCpp
        lnlohmann_json)