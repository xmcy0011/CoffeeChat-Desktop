#cmake_minimum_required(VERSION 3.14)
#project(chatkit VERSION 0.1 LANGUAGES CXX)

set(CHAT_KIT_LIB chatkit)

set(CHAT_KIT_SRC
        cim/base
        cim/core
        cim/crypt
        cim/db
        cim/pb
        cim/cim_def.h
        cim/cim_def.cpp
        cim/cim_dll.h
        cim/cim_helper.h
        cim/cim_helper.cpp
        cim/cim.h
        )

# CONFIG 会自动进行include，挺方便的
find_package(Protobuf CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(SQLiteCpp CONFIG REQUIRED)

include_directories(.)

# 动态库
add_library(${CHAT_KIT_LIB} ${CHAT_KIT_SRC})
#add_library(${CHAT_KIT_LIB} STATIC)

target_link_libraries(${CHAT_KIT_LIB} PRIVATE
        protobuf::libprotobuf-lite
        spdlog::spdlog_header_only
        SQLiteCpp)