cmake_minimum_required(VERSION 3.6)
project(RayTracing)
set(CMAKE_CXX_FLAGS "-O3")
set(CMAKE_CXX_STANDARD 11)

include_directories(vendor/concurrentqueue)
set(SOURCE_FILES src/main.cpp src/Vector.h src/common.h src/Ray.h src/Object.h src/Object.cpp src/Scene.cpp src/Scene.h src/Engine.cpp src/Engine.h src/Light.cpp src/Light.h)
add_executable(RayTracing ${SOURCE_FILES})