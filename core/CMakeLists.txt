cmake_minimum_required(VERSION 3.18.4)

project(WallpaperChanger-core LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(WallpaperChanger-core
            WallpaperChanger-core.cpp
            GlobTools.cpp
            WallpaperManager.cpp
)

install(TARGETS WallpaperChanger-core RUNTIME DESTINATION build)

