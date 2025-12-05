//
// Created by mikke on 18/11/2025.
//

<<<<<<< Updated upstream
#include "assignLot.h"
=======
#include "assignLot.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



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

void parkingMenu(struct parking_lot parkinglots[], int lotCount, struct car cars[], int carCount) {
    int choice;

    while (1) {
        printf("\n===== PARKING MENU =====\n");
        printf("1. Assign a car to a lot\n");
        printf("2. Deassign a car from a lot\n");
        printf("3. Look up a car by number plate\n");
        printf("4. Show all parking lots\n");
        printf("5. Add a new car\n");
        printf("6. Exit\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            // Handle non-number input
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n');  // Clear input buffer
            continue;
        }

        if (choice == 1) {
            // Assign parkingspace
            int carIndex;

            printf("Enter car index (0 - %d): ", carCount - 1);
            if (scanf("%d", &carIndex) != 1 || carIndex < 0 || carIndex >= carCount) {
                printf("Invalid car index.\n");
                while (getchar() != '\n');
                continue;
            }

            int lotIndex = checkOccupation(parkinglots, lotCount, cars[carIndex]);

            if (lotIndex == -1) {
                printf("No available lot for this car type.\n");
            } else {
                assignSpace(&parkinglots[lotIndex], &cars[carIndex]);
                cars[carIndex].assigned_lot = parkinglots[lotIndex].number;
                printf("Car %s assigned to lot %d.\n",
                       cars[carIndex].number_plate,
                       parkinglots[lotIndex].number);
            }

        } else if (choice == 2) {
            // Deassign parking space
            int lotIndex;

            printf("Enter lot index (0-%d): ", lotCount - 1);
            if (scanf("%d", &lotIndex) != 1 || lotIndex < 0 || lotIndex >= lotCount) {
                printf("Invalid lot index.\n");
                while (getchar() != '\n');
                continue;
            }

            if (parkinglots[lotIndex].status == VACANT) {
                printf("That lot is already vacant.\n");
                continue;
            }

            int carFound = 0;

            for (int i = 0; i < carCount; i++) {
                if (cars[i].assigned_lot == parkinglots[lotIndex].number) {

                    printf("Removing car %s from system...\n", cars[i].number_plate);

                    // Gør pladsen ledig igen
                    deAssignSpace(&parkinglots[lotIndex], &cars[i]);

                    // Fjern bilen fra array
                    cars = removeCar(cars, &carCount, i);

                    printf("Car removed successfully. New car count: %d\n", carCount);

                    carFound = 1;
                    break;
                }
            }

            if (!carFound)
                printf("No car found for this lot.\n");

        } else if (choice == 3) {
            // Få information omkring bil
            getAssignedLotOfCar(parkinglots, lotCount, cars, carCount);

        } else if (choice == 4) {
            // Print status af alle parkeringspladser
            for (int i = 0; i < lotCount; i++) {
                printf("Lot %d | Floor %d | Zone %s | Number %d | Type %s | Status %s\n",
                       i,
                       parkinglots[i].floor,
                       parkinglots[i].zone,
                       parkinglots[i].number,
                       typeToString(parkinglots[i].type),
                       statusToString(parkinglots[i].status));
            }

        } else if (choice == 5) {
            cars = addCar(cars, &carCount);
            autoAssignNewCar(&cars[carCount - 1], parkinglots, lotCount);
        }
        else if (choice == 6) {
            printf("Exiting...\n");
            break;
        } else {
            printf("Invalid option. Try again.\n");
        }
    }
}
struct car* addCar(struct car *cars, int *carCount) {
    char plate[16];
    int type;

    printf("Enter number plate:");
    scanf("%15s", plate);

    printf("Car type (0 = STANDARD, 1 = HANDICAPPED, 2 = EV): ");
    scanf("%d", &type);

    // Check at rigtig type er indtastet
    if (type < 0 || type > 2) {
        printf("Invalid type. Car not added.\n");
        return cars;
    }

    // Resize array, +1 bil
    struct car *newCars = realloc(cars, (*carCount + 1) * sizeof(struct car));

    if (newCars == NULL) {
        printf("Adding car failed. Car not added.\n");
        return cars;
    }

    cars = newCars;

    // Tilføj bil til listen
    strcpy(cars[*carCount].number_plate, plate);
    cars[*carCount].type = (enum type)type;
    cars[*carCount].assigned_lot = -1;

    (*carCount)++;

    printf("Car %s added successfully!\n", plate);

    return cars;
}

struct car* removeCar(struct car *cars, int *carCount, int indexToRemove) {
    if (*carCount == 0) return cars;

        // Flyt alle elementer 1 plads til venstre
    for (int i = indexToRemove; i < *carCount - 1; i++) {
        cars[i] = cars[i + 1];
    }

    (*carCount)--;

    // Gør array 1 mindre i størrelse
    struct car *newArr = realloc(cars, (*carCount) * sizeof(struct car));

    if (newArr == NULL && *carCount > 0) {
        printf("Removing car failed, returning old list.\n");
        return cars;
    }

    return newArr ? newArr : cars;
}

void autoAssignNewCar(struct car *newCar,
                      struct parking_lot parkinglots[],
                      int lotCount)
{
    int freeIndex = checkOccupation(parkinglots, lotCount, *newCar);

    if (freeIndex == -1) {
        printf("No free compatible parking lot available for this car.\n");
        newCar->assigned_lot = -1;
        return;
    }

    assignSpace(&parkinglots[freeIndex], newCar);

    printf("Car %s assigned to lot number %d (index %d)\n",
           newCar->number_plate,
           parkinglots[freeIndex].number,
           freeIndex);
}
>>>>>>> Stashed changes
