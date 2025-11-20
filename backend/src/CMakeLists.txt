cmake_minimum_required(VERSION 4.0)
add_library(parkingLot
        assignLot.c
        parkingLot.c
        parkingLotData.c
        ../testing/testassignLot.c
)

target_include_directories(parkingLot PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(backend main.c)

target_link_libraries(backend parkingLot)


