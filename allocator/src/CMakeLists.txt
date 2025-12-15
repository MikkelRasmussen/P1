add_library(parkingLot parkingLot.c
        parkingLotData.c)
target_include_directories(parkingLot PUBLIC .)

add_executable(backend main.c
        parkingLotData.c
        assignLot.c
        assignLot.h
)
add_executable(testing parkingLotData.c
        assignLot.c
        assignLot.h
)

include(../mtest/mtest.cmake)
target_link_libraries(backend m parkingLot)


