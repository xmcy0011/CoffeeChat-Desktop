project(hellochat)

set(HELLO_CHAT_VERSION 1.0)
set(HELLO_CHAT_VERSION_MARJOR 1)
set(HELLO_CHAT_VERSION_MINOR 0)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# The Qt SDK path
set(QT_SDK_DIR "/Users/xuyc/Qt/5.12.11/clang_64/")
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${QT_SDK_DIR})

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Quick REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick REQUIRED)

set(PROJECT_SOURCES
        main.cpp
        qml.qrc
        )

if (${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(hellochat
            MANUAL_FINALIZATION
            ${PROJECT_SOURCES}
            )
else ()
    if (ANDROID)
        add_library(hellochat SHARED
                ${PROJECT_SOURCES}
                )
    else ()
        add_executable(hellochat
                ${PROJECT_SOURCES}
                )
    endif ()
endif ()

target_compile_definitions(hellochat
        PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(hellochat
        PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick)

set_target_properties(hellochat PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER cim.sdk.com
        MACOSX_BUNDLE_BUNDLE_VERSION ${HELLO_CHAT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${HELLO_CHAT_VERSION_MARJOR}.${HELLO_CHAT_VERSION_MINOR}
        )

if (QT_VERSION_MAJOR EQUAL 6)
    qt_import_qml_plugins(hellochat)
    qt_finalize_executable(hellochat)
endif ()
