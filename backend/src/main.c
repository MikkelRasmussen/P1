#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parkingLot.h"
#include "assignLot.h"


    int main() {

        //Få længden af filen over parkeringspladser
        int fileLength = lengthOfDataFile();
        //printf("File Length: %d\n", fileLength);

        //Opret en liste af parkeringspladser
        struct parking_lot parkinglots[fileLength];

        //Indlæser alt dataen fra filen ind i listen af parkeringspladser.
        createParkingLot(fileLength, parkinglots);

        /* struct car car1 = {"ABC123", STANDARD, -1};
        struct car car2 = {"XYZ789", EV, -1};
        struct car car3 = {"LMN456", HANDICAPPED, -1};
        struct car car4 = {"QWE111", STANDARD, -1};
        struct car car5 = {"RTY222", EV, -1};
        struct car car6 = {"UIO333", HANDICAPPED, -1};
            parkinglots[0].type = HANDICAPPED;
            assignSpace(&parkinglots[7], &car1);
            assignSpace(&parkinglots[1], &car2);
            assignSpace(&parkinglots[2], &car3);
            assignSpace(&parkinglots[3], &car4);
            assignSpace(&parkinglots[4], &car5);

            int gg = checkOccupation(parkinglots, fileLength, car6);
            printf("Lot %d is vacant \n", gg);

            printf("Car is assigned spot %d \n" , car6.assigned_lot); */

        return 0;
    }

