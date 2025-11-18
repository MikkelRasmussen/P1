#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parkingLot.h"


    int main() {

        //Få længden af filen over parkeringspladser
        int fileLength = lengthOfDataFile();
        //printf("File Length: %d\n", fileLength);

        //Opret en liste af parkeringspladser
        struct parking_lot parkinglots[fileLength];

        //Indlæser alt dataen fra filen ind i listen af parkeringspladser.
        createParkingLot(fileLength, parkinglots);

        return 0;
    }

