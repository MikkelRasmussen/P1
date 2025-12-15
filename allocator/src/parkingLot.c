//
// Created by mikkel on 14/11/2025.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parkingLot.h"

#include <stdbool.h>

// Creates parking lot structures from data file containing parking structere info
void createParkingLot(int size, struct parking_lot parkinglots[])
{
    // Loop through each line in the data file and scan data into parking lot structures
    for (int i = 0; i < size; i++)
    {
        // Using scanDataFile function to create each parking lot structure
        scanDataFile(i + 1, &parkinglots[i].floor, parkinglots[i].zone, &parkinglots[i].number, &parkinglots[i].type, &parkinglots[i].status);
        parkinglots[i].status = VACANT; // Initially vacant - COMMENTED OUT WHEN SIMULATING
        parkinglots[i].id = i;
    }
}

// Converts status enum to string for display
const char *statusToString(enum status s)
{
    switch (s)
    {
    case VACANT:
        return "VACANT";
    case OCCUPIED:
        return "OCCUPIED";
    case ERROR:
        return "ERROR";
    default:
        return "UNKNOWN";
    }
}

// Converts type enum to string for display
const char *typeToString(enum type t)
{
    switch (t)
    {
    case HANDICAPPED:
        return "HANDICAPPED";
    case EV:
        return "EV";
    case STANDARD:
        return "STANDARD";
    default:
        return "UNKNOWN";
    }
}

// Looks up a car by number plate and displays its assigned lot information
int getAssignedLotOfCar(struct parking_lot lots[], int lotCount, struct car cars[], int carCount)
{
    char plate[16];
    while (getchar() != '\n')
        ;
    printf("Enter number plate: ");
    scanf("%15s", plate);

    // Loop through cars to find matching number plate
    for (int i = 0; i < carCount; i++)
    {
        // Check for matching plate
        if (strcmp(plate, cars[i].number_plate) == 0)
        {

            int lotNumber = cars[i].assigned_lot;

            // if the car is not assigned to any lot
            if (lotNumber == -1)
            {
                printf("Car %s is not assigned to any lot.\n", plate);
                return 0;
            }

            // Print assigned lot information
            for (int j = 0; j < lotCount; j++)
            {
                if (lots[j].number == lotNumber)
                {

                    printf("\n=== Car Found ===\n");
                    printf("Number Plate: %s\n", cars[i].number_plate);
                    printf("Car Type: %s\n", typeToString(cars[i].type));

                    printf("\n--- Assigned Lot ---\n");
                    printf("Lot Index:   %d\n", j);
                    printf("Lot Number:  %d\n", lots[j].number);
                    printf("Floor:       %d\n", lots[j].floor);
                    printf("Zone:        %s\n", lots[j].zone);
                    printf("Type:        %s\n", typeToString(lots[j].type));
                    printf("Status:      %s\n\n", statusToString(lots[j].status));

                    return 0;
                }
            }
            return 0;
        }
    }

    // Handle case where no matching car is found
    printf("No car with that plate exists.\n");
    return -1;
}
