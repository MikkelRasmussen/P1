#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parkingLot.h"
#include "assignLot.h"


    int main() {

        //Få længden af filen over parkeringspladser
        int fileLength = lengthOfDataFile();
        printf("File Length: %d\n", fileLength);

        //Opret en liste af parkeringspladser
        struct parking_lot parkinglots[fileLength-1];

        //Indlæser alt dataen fra filen ind i listen af parkeringspladser.
        createParkingLot(fileLength, parkinglots);

        int carCount = 4;
        struct car *cars = malloc(carCount * sizeof(struct car));
        cars[0] = (struct car){"LMN456", HANDICAPPED, -1};
        cars[1] = (struct car){"QWE111", STANDARD, -1};
        cars[2] = (struct car){"RTY222", EV, -1};
        cars[3] = (struct car){"UIO333", HANDICAPPED, -1};

        parkingMenu(parkinglots, fileLength-1, cars, carCount);
        free(cars);

        return 0;
    }

