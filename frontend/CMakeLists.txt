cmake_minimum_required(VERSION 3.15)
project(main C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)

configure_file(${CMAKE_SOURCE_DIR}/icon.png ${CMAKE_BINARY_DIR}/icon.png COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/style.rgs ${CMAKE_BINARY_DIR}/style.rgs COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/icons.rgi ${CMAKE_BINARY_DIR}/icons.rgi COPYONLY)

find_package(raylib CONFIG REQUIRED)
find_package(nfd CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

# ONLY add subfolders ONCE
add_subdirectory(src)
add_subdirectory(testing)

enable_testing()
