cmake_minimum_required(VERSION 3.15)
project(main C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)

configure_file(${CMAKE_SOURCE_DIR}/icon.png ${CMAKE_BINARY_DIR}/icon.png COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/style.rgs ${CMAKE_BINARY_DIR}/style.rgs COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/icons.rgi ${CMAKE_BINARY_DIR}/icons.rgi COPYONLY)

find_package(raylib CONFIG REQUIRED)
find_package(nfd CONFIG REQUIRED)
add_subdirectory(src)
add_subdirectory(src/utils)
add_subdirectory(src/parking-spot)
add_subdirectory(src/project)
add_subdirectory(src/car)
add_subdirectory(src/camera)

enable_testing()
add_subdirectory(testing)