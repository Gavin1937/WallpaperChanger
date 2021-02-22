cmake_minimum_required(VERSION 3.18.0)

project(WallpaperChanger-gui LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


add_definitions(-DUNICODE -D_UNICODE)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

set(CMAKE_PREFIX_PATH "C:/Qt/6.0.0/msvc2019_64")

find_package(Qt6 COMPONENTS Widgets REQUIRED)

file(GLOB SRC
    WallpaperChanger-gui.cpp
    mainwindow.cpp
    timer.cpp
    ConfigManager.cpp
    GlobTools.cpp
)
file(GLOB HEADER
    mainwindow.h
    timer.h
    resources.h
    ConfigManager.h
    GlobTools.h
)
file(GLOB RES
    resources.qrc
    resources.rc
)

add_executable(WallpaperChanger-gui
    WIN32
    ${SRC} ${HEADER} ${RES}
)


target_link_libraries(WallpaperChanger-gui Qt6::Widgets)
set_target_properties(WallpaperChanger-gui PROPERTIES
    LINK_FLAGS /SUBSYSTEM:WINDOWS
)

install(TARGETS WallpaperChanger-gui RUNTIME DESTINATION build)
