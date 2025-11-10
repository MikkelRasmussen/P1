cmake_minimum_required(VERSION 3.15)
project(frontend C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)

find_package(raylib CONFIG REQUIRED)
add_subdirectory(src)