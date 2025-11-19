dd_executable(testassignLot testassignLot.c)

target_link_libraries(testassignLot testassignLot mtest)

add_test(NAME testassignLot
        COMMAND testassignLot)
