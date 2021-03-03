cmake_minimum_required(VERSION 3.18.0)

project(WallpaperChanger-core LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DUNICODE -D_UNICODE)

set(SRC
    WallpaperChanger-core.cpp
    ArgumentHandler.cpp
    WallpaperManager.cpp
)
set(HEADER
    ArgumentHandler.h
    WallpaperManager.h
)
set(UTIL
    ../utilities/CacheRW.h
    ../utilities/CacheRW.cpp
    ../utilities/ConfigManager.h
    ../utilities/ConfigManager.cpp
    ../utilities/GlobTools.h
    ../utilities/GlobTools.cpp
)

add_executable(WallpaperChanger-core
    WIN32
    ${SRC} ${HEADER} ${UTIL}
)

install(TARGETS WallpaperChanger-core RUNTIME DESTINATION build)

