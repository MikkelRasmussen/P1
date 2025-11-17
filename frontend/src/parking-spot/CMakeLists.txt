add_library(parking_spot
    parking-spot.c
    parking-spot.h
)

target_include_directories(parking_spot PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link raylib because parking-spot.c includes raylib.h
target_link_libraries(parking_spot PUBLIC raylib)
