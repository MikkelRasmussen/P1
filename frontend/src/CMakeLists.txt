add_subdirectory(utils)
add_subdirectory(parking-spot)
add_subdirectory(parking-lot)
add_subdirectory(car)

add_executable(frontend main.c)
target_link_libraries(frontend PRIVATE raylib utils)