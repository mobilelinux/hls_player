cmake_minimum_required(VERSION 3.10)
project(hls_player)

set(CMAKE_C_STANDARD 99)

# AWTK Path
set(AWTK_ROOT "${CMAKE_SOURCE_DIR}/3rd/awtk" CACHE PATH "AWTK root directory")
set(AWTK_MVVM_ROOT "${CMAKE_SOURCE_DIR}/3rd/awtk-mvvm" CACHE PATH "AWTK MVVM root directory")

# Include AWTK
list(APPEND CMAKE_MODULE_PATH "${AWTK_ROOT}/cmake")

# Find FFmpeg and SDL2
find_package(PkgConfig REQUIRED)
pkg_check_modules(FFMPEG REQUIRED libavformat libavcodec libavutil libswscale)
pkg_check_modules(SDL2 REQUIRED sdl2)

include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${AWTK_ROOT}/src
    ${AWTK_ROOT}/src/ext_widgets
    ${AWTK_MVVM_ROOT}/src
    ${FFMPEG_INCLUDE_DIRS}
    ${SDL2_INCLUDE_DIRS}
)

link_directories(
    ${FFMPEG_LIBRARY_DIRS}
    ${SDL2_LIBRARY_DIRS}
)

file(GLOB_RECURSE SOURCES 
    "src/*.c" 
    "src/*.h"
)

# Exclude AWTK/MVVM sources from GLOB if we are adding them as subdirectories or libs
# For simplicity, we might need to compile AWTK sources if not using a prebuilt lib.
# Usually AWTK projects use scons, but CMake is also supported.
# Let's assume we link against awtk static lib or include source.

# For this task, I will try to include AWTK as a subdirectory if it exists, 
# otherwise I might need to fetch it.
# But standard practice: User provides AWTK. 
# I will assume we need to fetch it.

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME}
    ${FFMPEG_LIBRARIES}
    ${SDL2_LIBRARIES}
    awtk
    awtk-mvvm
)
