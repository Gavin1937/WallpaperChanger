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

include_directories(ui)

set(SRC
    WallpaperChanger-gui.cpp
    mainwindow.cpp
    mainwindow_TrayIcon.cpp
    mainwindow_WallpaperTab.cpp
    mainwindow_SettingTab.cpp
    timer.cpp
    CacheBrowserDlg.cpp
    ImageEditor.cpp
    ImageEditorDlg.cpp
)
set(HEADER
    mainwindow.h
    timer.h
    CacheBrowserDlg.h
    ImageEditor.h
    ImageEditorDlg.h
    Qt_CustomEventID.h
    ProgramVersion.h
    resources.h
)
set(RES
    resources.qrc
    resources.rc
)
set(UTIL
    ../utilities/CacheRW.h
    ../utilities/CacheRW.cpp
    ../utilities/ConfigManager.h
    ../utilities/ConfigManager.cpp
    ../utilities/GlobTools.h
    ../utilities/GlobTools.cpp
    ../utilities/AppLaunchHandler.h
    ../utilities/AppLaunchHandler.cpp
)
set(UI
    ./ui/mainwindow.ui
    ./ui/cachebrowserdlg.ui
)

# wrap qt .ui file
if(NOT USE_UIC)
    if(qt_version EQUAL 6)
        qt_wrap_ui(${WallpaperChanger-gui}_UI_Hdrs ${UI})
    endif()
endif()

add_executable(WallpaperChanger-gui
    WIN32
    ${SRC} ${HEADER} ${RES} ${UTIL} ${UI}
)


target_link_libraries(WallpaperChanger-gui Qt6::Widgets)
set_target_properties(WallpaperChanger-gui PROPERTIES
    LINK_FLAGS /SUBSYSTEM:WINDOWS
)

install(TARGETS WallpaperChanger-gui RUNTIME DESTINATION build)
