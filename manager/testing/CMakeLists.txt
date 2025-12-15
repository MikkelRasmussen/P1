add_executable(testing testing.c)
add_executable(noah noah/noah.c)
add_executable(jeppe jeppe/jeppe.c)
add_executable(bugge bugge/bugge.c)

target_link_libraries(testing PRIVATE raylib utils)