add_library(parkingLot parkingLot.c
        parkingLotData.c)
target_include_directories(parkingLot PUBLIC .)

add_executable(backend main.c
        parkingLotData.c
)
target_link_libraries(backend m parkingLot)


