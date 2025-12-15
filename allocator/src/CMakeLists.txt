add_library(parkingLot parkingLot.c
        parkingLotData.c)
target_include_directories(parkingLot PUBLIC .)

add_executable(allocator main.c
        parkingLotData.c
        assignLot.c
        assignLot.h
)

target_link_libraries(allocator m parkingLot)


