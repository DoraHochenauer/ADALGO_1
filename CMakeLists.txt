cmake_minimum_required(VERSION 4.3)
project(ADALGO)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_Declare(raylib
        URL https://github.com/raysan5/raylib/archive/refs/tags/5.5.tar.gz)
FetchContent_MakeAvailable(raylib)

add_executable(ADALGO main.cpp
        PointCloud.cpp
        PointCloud.h
        enums.h
        randomGenerator.cpp
        randomGenerator.h
        Point.cpp
        Point.h
        Painter.cpp
        Painter.h
        ConvexHull.cpp
        ConvexHull.h
        GiftWrapping.cpp
        GiftWrapping.h)
target_link_libraries(ADALGO raylib)

# Projektordner als Konstante, damit Dateien wie importPoints auch gefunden werden,
# wenn das Programm aus cmake-build-debug gestartet wird
target_compile_definitions(ADALGO PRIVATE PROJECT_DIR="${CMAKE_SOURCE_DIR}")
