project(chatkit)

# 库名字和版本号设置
set(ChatKitLib chatkit)
set(ChatKitLib_VERSION 3.4.1)

# 源码文件
aux_source_directory(cim CHAT_KIT_SRC)
aux_source_directory(cim/base CHAT_KIT_SRC)
aux_source_directory(cim/base/file CHAT_KIT_SRC)
aux_source_directory(cim/base/util CHAT_KIT_SRC)
aux_source_directory(cim/core CHAT_KIT_SRC)
aux_source_directory(cim/crypt CHAT_KIT_SRC)
aux_source_directory(cim/db CHAT_KIT_SRC)
aux_source_directory(cim/pb CHAT_KIT_SRC)

if (APPLE) # 使用vcpkg 快速安装的依赖
    find_package(protobuf CONFIG REQUIRED)
    find_package(spdlog CONFIG REQUIRED)
    find_package(restclient-cpp CONFIG REQUIRED)
    find_package(SQLiteCpp CONFIG REQUIRED)
#    find_package(evpp CONFIG REQUIRED)
    find_package(nlohmann_json CONFIG REQUIRED)
else ()
    #set(VCPKG_INCLUDE_DIR /Users/xuyc/repo/github/vcpkg/installed/x64-osx/include)
    #set(VCPKG_LIB_DIR /Users/xuyc/repo/github/vcpkg/installed/x64-osx/lib)
endif ()

# 动态库
add_library(${ChatKitLib} SHARED ${CHAT_KIT_SRC})
#add_library(${ChatKitLib} STATIC)
# 起一个别名
add_library(cim::chatkit ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
        PUBLIC
        ${PROJECT_SOURCE_DIR}
        ${VCPKG_INCLUDE_DIR}
        )
target_link_directories(${PROJECT_NAME}
        PUBLIC
        ${VCPKG_LIB_DIR})

target_link_libraries(${ChatKitLib}
        PRIVATE
        # 在macOS下，是静态库，windiws下，需要显式安装静态库，否则为动态库
        protobuf::libprotobuf-lite
        restclient-cpp
        SQLiteCpp
        spdlog::spdlog
        nlohmann_json::nlohmann_json
        )
# 静态库
#        libprotobuf-lite.a
#        spdlog.a
#        evpp libfmt.a libssl.a libcrypto.a
#        SQLiteCpp.a libsqlite3.a
#        librestclient-cpp.a libcurl.a)