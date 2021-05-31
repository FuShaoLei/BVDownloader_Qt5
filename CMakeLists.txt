cmake_minimum_required(VERSION 3.17)

project(BVDownloader_Qt5 VERSION 1.0.2 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "-mwindows")

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Network REQUIRED)

set(PROJECT_SOURCES
        main.cc
        mainwindow.cc
        mainwindow.hh
        mainwindow.ui
        urllist.hh
        urllist.cc
        urllist.ui
        downloader.hh
        downloader.cc
)

add_executable(BVDownloader_Qt5
    ${PROJECT_SOURCES}
)

target_link_libraries(BVDownloader_Qt5 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(BVDownloader_Qt5 PRIVATE Qt${QT_VERSION_MAJOR}::Network)
