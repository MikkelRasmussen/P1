//
// Created by mikke on 18/11/2025.
//

#include "assignLot.h"
#include <stdio.h>


void assignSpace(struct parking_lot *lot, struct car *car) {
    car->assigned_lot = lot->number;  //
    lot->status = OCCUPIED;
}

void deAssignSpace (struct parking_lot *lot, struct car *car) {
    lot->status = VACANT;
    car->assigned_lot = -1;
}

void reAssignSpace(struct parking_lot *lot, struct car *car, int check ) {
    lot-> status = ERROR;
    car->assigned_lot = lot->number;
}

int checkOccupation(struct parking_lot parking_lot[], int length, struct car car) {
    for (int i = 0; i <= length; i++) {
        if (parking_lot[i].status == VACANT ) {
            if (parking_lot[i].type == STANDARD) {
                return i;
            }
            else if (parking_lot[i].type == car.type) {
                return i;
            }
        }
    }
    printf("All spaces is Occupied");
    return -1;
}