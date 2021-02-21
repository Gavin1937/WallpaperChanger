cmake_minimum_required(VERSION 3.18.0)

project(WallpaperChanger-core LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DUNICODE -D_UNICODE)

file(GLOB SRC
    WallpaperChanger-core.cpp
    ArgumentHandler.cpp
    GlobTools.cpp
    WallpaperManager.cpp
    ConfigManager.cpp
)
file(GLOB HEADER
    ArgumentHandler.h
    GlobTools.h
    WallpaperManager.h
    ConfigManager.h
)

add_executable(WallpaperChanger-core
    WIN32
    ${SRC} ${HEADER}
)

install(TARGETS WallpaperChanger-core RUNTIME DESTINATION build)

