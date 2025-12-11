add_library(camera
    camera.c
    camera.h
)

target_include_directories(camera PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link raylib because camera.c/camera.h include raylib.h
target_link_libraries(camera PUBLIC raylib)
