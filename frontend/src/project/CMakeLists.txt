add_library(project
    project.c
    project.h
)

target_include_directories(project PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link nfd because project.c/project.h include nfd.h
target_link_libraries(project 
    PRIVATE 
    nfd::nfd 
    PUBLIC 
    utils
)
