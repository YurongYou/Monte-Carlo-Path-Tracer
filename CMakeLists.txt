cmake_minimum_required(VERSION 3.5.1)
project(RayTracing)

set(CMAKE_CXX_FLAGS "-O3 -pthread")
set(CMAKE_CXX_STANDARD 11)

find_package(PNG REQUIRED)
include_directories(${PNG_INCLUDE_DIR})

include_directories(vendor/concurrentqueue)
set(SOURCE_FILES src/main.cpp src/utility/Vector.h src/utility/common.h src/utility/Ray.h src/object/Object.h src/object/Object.cpp src/Scene.cpp src/Scene.h src/Engine.cpp src/Engine.h src/utility/ObjLoader.h src/object/Texture.h)

add_executable(RayTracing ${SOURCE_FILES})
target_link_libraries(RayTracing ${PNG_LIBRARY})