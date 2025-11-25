cmake_minimum_required(VERSION 3.15)
project(frontend C)

# Tell CMake to use vcpkg installed libraries
set(CMAKE_TOOLCHAIN_FILE "C:/Users/jdj10/Documents/Programmering/vcpkg/installed/x64-windows/include" CACHE STRING "")

# Find external libraries
find_package(raylib CONFIG REQUIRED)
find_package(nfd CONFIG REQUIRED)

# Main executable
add_executable(main
    main.c
)

# Add subdirectories for your own libraries
add_subdirectory(utils)
add_subdirectory(project)
add_subdirectory(camera)

# Link libraries to main
target_link_libraries(main
    PRIVATE
        raylib
        nfd::nfd
        utils
        project
        camera
)
