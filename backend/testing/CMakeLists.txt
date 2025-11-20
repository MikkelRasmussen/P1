add_executable(run_tests testassignLot.c
        ../src/assignLot.c
        ../src/parkingLot.c
        ../src/parkingLotData.c
)

target_include_directories(run_tests PRIVATE
        ../mtest/include
        ../src
)
target_link_libraries(run_tests mtest)