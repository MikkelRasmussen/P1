add_executable(testing testing.c)
target_link_libraries(testing mtest m parkingLot)

discover_tests(testing)
