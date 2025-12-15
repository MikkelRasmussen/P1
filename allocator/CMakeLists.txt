cmake_minimum_required(VERSION 4.0)
project(backend C)

set(CMAKE_C_STANDARD 11)

add_subdirectory(src)
add_subdirectory(mtest)
enable_testing()

