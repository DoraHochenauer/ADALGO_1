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
        randomGenerator.h)
target_link_libraries(ADALGO raylib)
