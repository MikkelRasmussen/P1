//
// Created by mikke on 18/11/2025.
//

#ifndef BACKEND_ASSIGNLOT_H
#define BACKEND_ASSIGNLOT_H

#include "parkingLot.h"

void assignSpace(struct parking_lot *lot, struct car *car);
void deAssignSpace(struct parking_lot *lot, struct car *car);
void reAssignSpace(struct parking_lot *lot, struct car *car, int check);

int checkOccupation(struct parking_lot[], int length, struct car car);

#endif //BACKEND_ASSIGNLOT_H

