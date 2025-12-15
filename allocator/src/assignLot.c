//
// Created by mikke on 18/11/2025.
//

#include "assignLot.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Assigns a parking lot to a car and updates the lot status
void assignSpace(struct parking_lot *lot, struct car *car)
{
    car->assigned_lot = lot->id; //
    lot->status = OCCUPIED;
}

// Deassigns a parking lot from a car and updates the lot status
void deAssignSpace(struct parking_lot *lot, struct car *car)
{
    lot->status = VACANT;
    car->assigned_lot = -1;
}

// Reassigns a parking lot to a car and updates the lot status to ERROR
void reAssignSpace(struct parking_lot *lot, struct car *car, int check)
{
    lot->status = ERROR;
    car->assigned_lot = lot->id;
}

// Checks for an available parking lot that matches the car type
int checkOccupation(struct parking_lot parking_lot[], int length, struct car car)
{
    // Loop through all parking lots
    for (int i = 0; i < length; i++)
    {
        // Check if lot is vacant
        if (parking_lot[i].status == VACANT)
        {
            // Check if lot type matches car type
            if (parking_lot[i].type == car.type)
            {
                return i;
            }
            // If parkinglot is STANDARD, any car can park there
            else if (parking_lot[i].type == STANDARD)
            {
                return i;
            }
        }
    }
    // If no suitable lot found, return -1
    printf("All spaces is Occupied");
    return -1;
}

// Displays a menu for parking operations and handles user input
void parkingMenu(struct parking_lot parkinglots[], int lotCount, struct car cars[], int carCount)
{
    int choice;

    while (1)
    {
        printf("\n===== PARKING MENU =====\n");
        printf("1. Assign a car to a lot\n");
        printf("2. Deassign a car from a lot\n");
        printf("3. Look up a car by number plate\n");
        printf("4. Show all parking lots\n");
        printf("5. Add a new car\n");
        printf("6. Exit\n");
        printf("Enter choice: ");

        // Read user choice
        if (scanf("%d", &choice) != 1)
        {
            // Handle non-number input
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n')
                ; // Clear input buffer
            continue;
        }

        // Assign parking lot
        if (choice == 1)
        {
            int carIndex;

            // Get car index from user
            printf("Enter car index (0 - %d): ", carCount - 1);
            if (scanf("%d", &carIndex) != 1 || carIndex < 0 || carIndex >= carCount)
            {
                // If invalid index, show error
                printf("Invalid car index.\n");
                while (getchar() != '\n')
                    ;
                continue;
            }

            // Check for available lot
            int lotIndex = checkOccupation(parkinglots, lotCount, cars[carIndex]);

            // If no lot available, show message
            if (lotIndex == -1)
            {
                printf("No available lot for this car type.\n");
            }
            // Assign car to lot
            else
            {
                assignSpace(&parkinglots[lotIndex], &cars[carIndex]);
                cars[carIndex].assigned_lot = parkinglots[lotIndex].id;
                printf("Car %s assigned to lot %d.\n",
                       cars[carIndex].number_plate,
                       parkinglots[lotIndex].number);
            }
        }

        // Deassign parking lot
        else if (choice == 2)
        {
            int lotIndex;

            // Get lot index from user
            printf("Enter lot index (0-%d): ", lotCount - 1);
            if (scanf("%d", &lotIndex) != 1 || lotIndex < 0 || lotIndex >= lotCount)
            {
                printf("Invalid lot index.\n");
                while (getchar() != '\n')
                    ;
                continue;
            }

            // Check if lot is already vacant
            if (parkinglots[lotIndex].status == VACANT)
            {
                printf("That lot is already vacant.\n");
                continue;
            }

            int carFound = 0;

            // Find the car assigned to this lot
            for (int i = 0; i < carCount; i++)
            {
                if (cars[i].assigned_lot == parkinglots[lotIndex].id)
                {

                    printf("Removing car %s from system...\n", cars[i].number_plate);

                    // Deassign the parking lot
                    deAssignSpace(&parkinglots[lotIndex], &cars[i]);

                    // Remove car from the list
                    cars = removeCar(cars, &carCount, i);

                    printf("Car removed successfully. New car count: %d\n", carCount);

                    carFound = 1;
                    break;
                }
            }
            // If no car found for the lot, show message
            if (!carFound)
                printf("No car found for this lot.\n");
        }
        // Look up car by number plate
        else if (choice == 3)
        {
            // Call function to look up car
            getAssignedLotOfCar(parkinglots, lotCount, cars, carCount);
        }
        // Show all parking lots
        else if (choice == 4)
        {
            for (int i = 0; i < lotCount; i++)
            {
                printf("Lot %d | Floor %d | Zone %s | Number %d | Type %s | Status %s\n",
                       i,
                       parkinglots[i].floor,
                       parkinglots[i].zone,
                       parkinglots[i].number,
                       typeToString(parkinglots[i].type),
                       statusToString(parkinglots[i].status));
            }
        }
        // Add a new car
        else if (choice == 5)
        {
            // Call function to add car
            cars = addCar(cars, &carCount);
            // Automatically assign the new car to a lot
            autoAssignNewCar(&cars[carCount - 1], parkinglots, lotCount);
        }
        // Exit the menu
        else if (choice == 6)
        {
            printf("Exiting...\n");
            break;
        }
        // Handle invalid options
        else
        {
            printf("Invalid option. Try again.\n");
        }
    }
}
// Adds a new car to the list and returns the updated list
struct car *addCar(struct car *cars, int *carCount)
{
    char plate[16];
    int type;

    // Get car details from user
    printf("Enter number plate:");
    scanf("%15s", plate);

    printf("Car type (0 = STANDARD, 1 = HANDICAPPED, 2 = EV): ");
    scanf("%d", &type);

    // Check for valid type
    if (type < 0 || type > 2)
    {
        printf("Invalid type. Car not added.\n");
        return cars;
    }

    // Resize array, +1 for new car
    struct car *newCars = realloc(cars, (*carCount + 1) * sizeof(struct car));

    if (newCars == NULL)
    {
        printf("Adding car failed. Car not added.\n");
        return cars;
    }

    // Update pointer to new array
    cars = newCars;

    // Add new car details
    strcpy(cars[*carCount].number_plate, plate);
    cars[*carCount].type = (enum type)type;
    cars[*carCount].assigned_lot = -1;

    *carCount++;

    printf("Car %s added successfully!\n", plate);

    // Return updated car list
    return cars;
}

// Removes a car from the list at the specified index and returns the updated list
struct car *removeCar(struct car *cars, int *carCount, int indexToRemove)
{
    if (*carCount == 0)
        return cars;

    // Move cars after the removed index one position to the left
    for (int i = indexToRemove; i < *carCount - 1; i++)
    {
        cars[i] = cars[i + 1];
    }

    (*carCount)--;

    // Make the array smaller
    struct car *newArr = realloc(cars, (*carCount) * sizeof(struct car));

    if (newArr == NULL && *carCount > 0)
    {
        printf("Removing car failed, returning old list.\n");
        return cars;
    }

    // Return updated car list
    return newArr ? newArr : cars;
}

// Automatically assigns a new car to an available parking lot
void autoAssignNewCar(struct car *newCar,
                      struct parking_lot parkinglots[],
                      int lotCount)
{
    // Check for available lot
    int freeIndex = checkOccupation(parkinglots, lotCount, *newCar);

    // If no lot available, show message
    if (freeIndex == -1)
    {
        printf("No free compatible parking lot available for this car.\n");
        newCar->assigned_lot = -1;
        return;
    }

    // Assign car to lot
    assignSpace(&parkinglots[freeIndex], newCar);

    // Display assignment details
    printf("Car %s assigned to lot number %d (index %d)\n",
           newCar->number_plate,
           parkinglots[freeIndex].id,
           freeIndex);
}

// Simulates the assignment time for cars and displays statistics
void simulateAssignmentTime(struct car cars[], int carCount,
                            struct parking_lot parkinglots[], int lotCount)
{

    double totalSimulatedSeconds = 0.0;

    // Loop through all cars
    for (int i = 0; i < carCount; i++)
    {
        // If car has no assigned lot, try to assign one
        if (cars[i].assigned_lot < 0)
        {
            autoAssignNewCar(&cars[i], parkinglots, lotCount);
        }

        double carTime = 20.0; // base time

        // If car has an assigned lot, add zone time
        if (cars[i].assigned_lot >= 0)
        {
            // Get the assigned lot
            struct parking_lot *lot = &parkinglots[cars[i].assigned_lot];
            // Get zone time
            double zoneTime = getZoneTime(lot->zone);
            // Add zone time to car time
            carTime += zoneTime;

            printf("Car %s assigned to lot %d, zone='%s', zoneTime=%.2f, totalCarTime=%.2f\n",
                   cars[i].number_plate,
                   lot->id,
                   lot->zone,
                   zoneTime,
                   carTime);
        }

        // Accumulate total simulated time
        totalSimulatedSeconds += carTime;
    }

    // Calculate average time per car
    double avgTime = (carCount > 0) ? totalSimulatedSeconds / carCount : 0.0;

    printf("\n====== SIMULATION RESULTS ======\n");
    printf("Cars processed: %d\n", carCount);
    printf("Total simulated time: %.2f seconds\n", totalSimulatedSeconds);
    printf("Average time per car: %.2f seconds\n", avgTime);
    printf("================================\n\n");
}

// Returns the time in seconds associated with a parking zone
double getZoneTime(const char *zone)
{
    if (strcmp(zone, "A") == 0)
        return 15.0;
    if (strcmp(zone, "B") == 0)
        return 30.0;
    if (strcmp(zone, "C") == 0)
        return 45.0;
    if (strcmp(zone, "D") == 0)
        return 60.0;
    return 10.0; // default base time
}