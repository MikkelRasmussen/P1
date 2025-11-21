//
// Created by mikkel on 14/11/2025.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parkingLot.h"


//Opretter en liste af parkeringspladser udfra ønsket antal og struct.
void createParkingLot(int size, struct parking_lot parkinglots[]) {
    for (int i = 0; i < size-1; i++){
        scanDataFile(i+1, &parkinglots[i].floor, parkinglots[i].area,&parkinglots[i].number);
        parkinglots[i].status = VACANT;    // Initially vacant
        parkinglots[i].type = STANDARD;    // Skal ændres når der bliver lagt typer i datafilenx

        printf("Lot %d | Floor: %d | Area: %s | Number: %d | Type: %s | Status: %s\n",
                       i,
                       parkinglots[i].floor,
                       parkinglots[i].area,
                       parkinglots[i].number,
                       typeToString(parkinglots[i].type),
                       statusToString(parkinglots[i].status));    }
}

const char* statusToString(enum status s) {
    switch (s) {
        case VACANT: return "VACANT";
        case OCCUPIED: return "OCCUPIED";
        case ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

const char* typeToString(enum type t) {
    switch (t) {
        case HANDICAPPED: return "HANDICAPPED";
        case EV: return "EV";
        case STANDARD: return "STANDARD";
        default: return "UNKNOWN";
    }
}

//Checker info indenfor indsatte parametre - skelet. Skal have fået funktionen til at indlæse data fra en fil.
car_info checkInfo(int parking_lot_ID,int car_type,char *license_plate,char *nationality){

    car_info information;
    information.parking_lot_ID = parking_lot_ID;
    information.c_Type = car_type;
    strcpy(information.license_plate, license_plate); strcpy(information.nationality, nationality);

    return information;
}