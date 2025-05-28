cmake_minimum_required(VERSION 3.10)
project(opengl-learning VERSION 0.0.1)
set(CMAKE_CXX_STANDARD 17)
include(E:/vcpkg/scripts/buildsystems/vcpkg.cmake)

find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)

add_subdirectory(demo1)
add_subdirectory(demo2)
add_subdirectory(demo3)
add_subdirectory(demo4)