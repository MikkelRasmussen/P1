//
// Created by mikkel on 14/11/2025.
//

#ifndef BACKEND_PARKINGLOT_H
#define BACKEND_PARKINGLOT_H

enum type {HANDICAPPED, EV, STANDARD};
enum status {VACANT, OCCUPIED, ERROR};
enum car_type {car_HANDICAPPED, car_EV, car_STANDARD};

struct parking_lot {
    int floor;
    char area[64];
    int number;
    enum type type;
    enum status status;
};

struct car_info {
    int parking_lot_ID;
    enum car_type c_Type;
    char license_plate[32], nationality[4];
};

typedef struct car_info car_info;

int lengthOfDataFile();
int scanDataFile(int id, int *floor, char *area, int *number);
void createParkingLot(int size, struct parking_lot parkinglots[]);

const char* statusToString(enum status s);
const char* typeToString(enum type t);

car_info checkInfo(int parking_lot_ID,int car_type,char *license_plate,char *nationality);

#endif //BACKEND_PARKINGLOT_H