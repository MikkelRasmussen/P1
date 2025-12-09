add_library(utils
    utils.c
    utils.h
)

# Include current folder for headers
target_include_directories(utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link nfd if needed (because utils might use project.h which includes nfd)
target_link_libraries(utils PUBLIC nfd::nfd)
