//
// Created by mikke on 18/11/2025.
//

#ifndef BACKEND_ASSIGNLOT_H
#define BACKEND_ASSIGNLOT_H

#include "parkingLot.h"

void assignSpace(struct parking_lot *lot, struct car *car);
void autoAssignNewCar(struct car *newCar, struct parking_lot parkinglots[], int lotCount);


int checkOccupation(struct parking_lot[], int length, struct car car);
void parkingMenu(struct parking_lot parkinglots[], int lotCount, struct car cars[], int carCount);

struct car* addCar(struct car *cars, int *carCount);
struct car* removeCar(struct car *cars, int *carCount, int indexToRemove);


#endif //BACKEND_ASSIGNLOT_H
