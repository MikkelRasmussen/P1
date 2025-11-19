//
// Created by mikkel on 14/11/2025.
//

#ifndef BACKEND_PARKINGLOT_H
#define BACKEND_PARKINGLOT_H

enum type {HANDICAPPED, EV, STANDARD};
enum status {VACANT, OCCUPIED, ERROR};

struct car {
    char number_plate[16];
    enum type type;
    int assigned_lot;
};

struct parking_lot {
    int floor;
    char area[64];
    int number;
    enum type type;
    enum status status;
};

int lengthOfDataFile();
int scanDataFile(int id, int *floor, char *area, int *number);
void createParkingLot(int size, struct parking_lot parkinglots[]);

const char* statusToString(enum status s);
const char* typeToString(enum type t);

#endif //BACKEND_PARKINGLOT_H