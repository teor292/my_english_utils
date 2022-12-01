cmake_minimum_required(VERSION 3.5)

project(gui_util VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Boost)
find_package(fmt)
find_package(spdlog)

file(GLOB PROJECT_SOURCES
    cereal/*.hpp
    cereal/archives/*.hpp
    cereal/types/*.hpp
    *.cpp
    *.h
    mainwindow.ui)

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    enable_language("RC")
    set (WIN32_RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/windows/resources.rc)
else()

    set(WIN32_RESOURCES "")

endif()


if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(gui_util
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ${WIN32_RESOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET gui_util APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(gui_util SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(gui_util
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_include_directories(gui_util PRIVATE
    "."
    ${Boost_INCLUDE_DIRS}
    ${fmt_INCLUDE_DIRS}
    ${spdlog_INCLUDE_DIRS}
)

target_compile_definitions(gui_util PRIVATE SPDLOG_FMT_EXTERNAL SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE)
target_link_libraries(gui_util PRIVATE Qt${QT_VERSION_MAJOR}::Widgets fmt::fmt spdlog::spdlog)



set_target_properties(gui_util PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS gui_util
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(gui_util)
endif()
