//
// Created by mikkel on 14/11/2025.
//

#ifndef BACKEND_PARKINGLOT_H
#define BACKEND_PARKINGLOT_H

enum type
{
    STANDARD,
    HANDICAPPED,
    EV
};
enum status
{
    VACANT,
    OCCUPIED,
    ERROR
};

struct parking_lot
{
    int id;
    int floor;
    char zone[4];
    int number;
    enum type type;
    enum status status;
};
struct car
{
    char number_plate[16];
    enum type type;
    int assigned_lot;
    int lot_index;
};

int lengthOfDataFile();
int scanDataFile(int id, int *floor, char *zone, int *number, enum type *type, enum status *status);
struct car *loadCarsFromCSV(const char *filename, int *carCount);
void createParkingLot(int size, struct parking_lot parkinglots[]);
int getAssignedLotOfCar(struct parking_lot lots[], int lotCount, struct car cars[], int carCount);

const char *statusToString(enum status s);
const char *typeToString(enum type t);

#endif // BACKEND_PARKINGLOT_H