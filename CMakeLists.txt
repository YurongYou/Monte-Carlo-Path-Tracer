cmake_minimum_required(VERSION 3.6)
project(RayTracing)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Vector.h constants.h Ray.h Object.h Object.cpp Scene.cpp Scene.h Engine.cpp Engine.h)
add_executable(RayTracing ${SOURCE_FILES})