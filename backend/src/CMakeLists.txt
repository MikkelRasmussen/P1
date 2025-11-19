add_library(parkingLot parkingLot.c
        parkingLotData.c assignLot.c)

target_include_directories(parkingLot PUBLIC .)

add_executable(backend main.c
)
target_link_libraries(backend m parkingLot)


