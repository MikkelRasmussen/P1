//
// Created by Frederik on 20-11-2025.
//
#include "../src/assignLot.h"
#include "../src/parkingLot.h"
#include "mtest.h"
#include <assert.h>

TEST_CASE(assignLot_test, {
  struct parking_lot lotArray[10];
  struct car car = {"ABC123", STANDARD, -1};
  // Intializing the two structs for the parking lot and the car itself for the
  // test case
  for (int i = 0; i < 10; i++) {
    lotArray[i].id = i;
    lotArray[i].floor = 1;
    strcpy(lotArray[i].zone, "A");
    lotArray[i].type = STANDARD;
    lotArray[i].status = VACANT;
  }

  assignSpace(&lotArray[0], &car);

  CHECK(car.assigned_lot == 0, "Car should be assigned lot 0");
  CHECK(lotArray[0].status == OCCUPIED, "lot number 0 should be occupied");
})

TEST_CASE(deAssignSpace_test, {
  struct parking_lot lotArray[10];
  struct car car = {"ABC123", STANDARD, 0};

  for (int i = 0; i < 10; i++) {
    lotArray[i].id = i;
    lotArray[i].floor = 1;
    strcpy(lotArray[i].zone, "A");
    lotArray[i].type = STANDARD;
    lotArray[i].status = OCCUPIED;
  }

  deAssignSpace(&lotArray[0], &car);

  CHECK(lotArray[0].status == VACANT, "Lot status changed to vacant");
  CHECK(car.assigned_lot == -1,
        "Car is deassigned from the space and returns -1");
})

TEST_CASE(reAssignSpace_test, {
  struct parking_lot lot = {1, 1, "A", 0, STANDARD, VACANT};
  struct car car = {"ABC123", STANDARD, -1};

  reAssignSpace(&lot, &car, 0);

  CHECK(lot.status == ERROR,
        "Lot status needs to be set to Error when reassigned");
  CHECK(car.assigned_lot == 0, "Car assigned lots is set to 0");
})

TEST_CASE(checkOccupation_test, {
  struct car car = {"ABC123", STANDARD, -1};
  struct parking_lot lotArray[10];

  for (int i = 0; i < 10; i++) {
    lotArray[0].floor = 1;
    strcpy(lotArray[0].zone, "A");
    lotArray[0].type = STANDARD;
    if (i < 3) {
      lotArray[i].status = OCCUPIED;
    } else {
      lotArray[i].status = VACANT;
    }
    lotArray[i].number = i;
  }
  int index = checkOccupation(lotArray, 9, car);

  CHECK(index == 3, "checkOccupation will skip over the first 3 lots, since "
                    "they are occupied and return index 3");
})

MAIN_RUN_TESTS(assignLot_test, deAssignSpace_test, reAssignSpace_test,
               checkOccupation_test)
