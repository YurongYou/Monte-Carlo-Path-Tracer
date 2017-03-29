cmake_minimum_required(VERSION 3.6)
project(RayTracing)
set(CMAKE_CXX_FLAGS "-O3")
set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Vector.h common.h Ray.h Object.h Object.cpp Scene.cpp Scene.h Engine.cpp Engine.h)
add_executable(RayTracing ${SOURCE_FILES})