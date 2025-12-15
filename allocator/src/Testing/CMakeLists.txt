add_executable(run_tests testassignLot.c
        ../../../../../../../Downloads/P1-TestingBackend/P1-TestingBackend/backend/src/assignLot.c
        ../../../../../../../Downloads/P1-TestingBackend/P1-TestingBackend/backend/src/parkingLot.c
        ../../../../../../../Downloads/P1-TestingBackend/P1-TestingBackend/backend/src/parkingLotData.c
)

target_include_directories(run_tests PRIVATE
        ../mtest/include
        ../../../../../../../Downloads/P1-TestingBackend/P1-TestingBackend/backend/src
)
target_link_libraries(run_tests mtest)