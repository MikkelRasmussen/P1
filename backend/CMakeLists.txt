cmake_minimum_required(VERSION 4.0)
project(backend C)

set(CMAKE_C_STANDARD 11)

add_subdirectory(src)
enable_testing()
add_subdirectory(Testing)

# Put `FetchContent_MakeAvailable(mtest)`, `add_subdirectory(mtest)` or `find_package(mtest)` here.
include(FetchContent)
FetchContent_Declare(mtest
        URL https://github.com/MortenSchou/mtest/archive/refs/tags/v0.2.0.zip
        URL_HASH SHA256=bec1e90fb00a5bc155de12ed5ed39ea5d1a1b6fcfb6c80cce5ad3e38e360248c
)
FetchContent_MakeAvailable(mtest)

if(NOT EXISTS "${PROJECT_SOURCE_DIR}/extern/mtest/CMakeLists.txt")
    message(FATAL_ERROR "The git submodules were not downloaded! GIT_SUBMODULE was turned off or failed. Did you forget to run `git submodule init` after cloning?")
else()
    add_subdirectory(extern/mtest)
endif()


add_executable(testassignLot testassignLot.c)
target_link_libraries(testassignLot mtest)

# Automatically discover test cases in my_example_test and add them to CTest.
discover_tests(testassignLot)