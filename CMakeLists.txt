cmake_minimum_required(VERSION 3.20)
project(swarenderer LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(swarenderer
    src/main.cpp
    src/App.cpp
    src/Mesh.cpp
)
target_include_directories(swarenderer PRIVATE include)
target_include_directories(swarenderer SYSTEM PRIVATE vendor)

target_compile_options(swarenderer PRIVATE /W4 /WX)
