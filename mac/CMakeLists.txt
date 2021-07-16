# your vcpkg path
#message("vcpkg in $ENV{VCPKGROOT}")
#set(CMAKE_TOOLCHAIN_FILE $ENV{VCPKGROOT}/vcpkg/scripts/buildsystems/vcpkg.cmake
#        CACHE STRING "Vcpkg toolchain file")

cmake_minimum_required(VERSION 3.14)

project(HelloChat VERSION 0.1 LANGUAGES CXX)

if (UNIX)
    if (NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE "Release")
    endif ()

    add_definitions(
            -g
            -fPIC
            -Wall
            -Werror # 警告视为错误，比较严格，可以根据需求启用
            -D__STDC_FORMAT_MACROS
            -Wno-unused-parameter
            -Wno-unused-function
            -Wunused-variable
            -Wunused-value
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
            )
endif ()

add_subdirectory(chatkit)

add_subdirectory(hellochat)