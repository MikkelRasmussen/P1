cmake_minimum_required(VERSION 3.16)
project(backend C)

set(CMAKE_C_STANDARD 11)

include(cmake/mtest.cmake)

add_subdirectory(src)
enable_testing()
add_subdirectory(mtest)

