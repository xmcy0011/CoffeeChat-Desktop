project(hellochat)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# 自动调用moc，uic，rcc处理qt的扩展部分
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 版本号设置
set(PROJECT_VERSION 1.0)
set(PROJECT_VERSION_MAJOR 1)
set(PROJECT_VERSION_MINOR 0)

# The Qt SDK path
set(QT_SDK_DIR "/Users/xuyc/Qt/5.12.11/clang_64/")
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${QT_SDK_DIR})

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        logindialog.h
        logindialog.cpp
        logindialog.ui
        resource.qrc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(hellochat
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET hellochat APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(hellochat SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(hellochat
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(hellochat PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(hellochat PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER cim.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(hellochat)
endif()
