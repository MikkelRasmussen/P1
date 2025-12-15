#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assignLot.h"
#include "parkingLot.h"

int main()
{

    // Get length of data file containing parking structere info
    int fileLength = lengthOfDataFile();
    printf("File Length: %d\n", fileLength);

    //  Create array to hold parking lots
    struct parking_lot parkinglots[fileLength - 1];

    // Call function to create parking lots
    createParkingLot(fileLength - 1, parkinglots);

    int carCount = 0;
    // Load cars from CSV file
    struct car *cars = loadCarsFromCSV("simulation1.csv", &carCount);
    if (!cars || carCount == 0)
    {
        printf("No cars to simulate.\n");
        return 1;
    }

    // Simulate assignment time for loaded cars (Uncomment to run simulation)
    // simulateAssignmentTime(cars, carCount, parkinglots, fileLength - 1);

    // Running the program with the parking menu
    parkingMenu(parkinglots, fileLength - 1, cars, carCount);
    free(cars);

    return 0;
}
