add_library(selection
    selection.c
    selection.h
)

target_include_directories(selection PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(selection 
    PRIVATE 
    raylib
    PUBLIC
    project
    utils
)
