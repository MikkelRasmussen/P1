//
// Created by mikkel on 14/11/2025.
//

#ifndef BACKEND_PARKINGLOT_H
#define BACKEND_PARKINGLOT_H

enum type {HANDICAPPED, EV, STANDARD};

struct parking_lot {
    int floor;
    char area[2];
    int number;
    enum type type;
    int vacant;
};

int lengthOfDataFile();
int scanDataFile();

void createParkingLot();

#endif //BACKEND_PARKINGLOT_H