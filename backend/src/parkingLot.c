    //
    // Created by mikkel on 14/11/2025.
    //

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "parkingLot.h"

    #include <stdbool.h>


    //Opretter en liste af parkeringspladser udfra ønsket antal og struct.
    void createParkingLot(int size, struct parking_lot parkinglots[]) {
        for (int i = 0; i < size-1; i++){
            scanDataFile(i+1, &parkinglots[i].floor, parkinglots[i].zone,&parkinglots[i].number, &parkinglots[i].type);
            parkinglots[i].status = VACANT;    // Initially vacant

            //Debugging ift parkerings struktur
            /* printf("Lot %d | Floor: %d | Area: %s | Number: %d | Type: %s | Status: %s\n",
                           i,
                           parkinglots[i].floor,
                           parkinglots[i].zone,
                           parkinglots[i].number,
                           typeToString(parkinglots[i].type),
                           statusToString(parkinglots[i].status));
                           */
                           }
    }

    const char* statusToString(enum status s) {
        switch (s) {
            case VACANT: return "VACANT";
            case OCCUPIED: return "OCCUPIED";
            case ERROR: return "ERROR";
            default: return "UNKNOWN";
        }
    }

    const char* typeToString(enum type t) {
        switch (t) {
            case HANDICAPPED: return "HANDICAPPED";
            case EV: return "EV";
            case STANDARD: return "STANDARD";
            default: return "UNKNOWN";
        }
    }


int getAssignedLotOfCar(struct parking_lot lots[], int lotCount, struct car cars[], int carCount)
    {
        char plate[16];
        while (getchar() != '\n');
        printf("Enter number plate: ");
        scanf("%15s", plate);

        // Søg gennem liste af biler udfra nummerplade
        for (int i = 0; i < carCount; i++) {
            //Check op den indtastede nummerplade matcher en nummerplade i listen
            if (strcmp(plate, cars[i].number_plate) == 0) {

                int lotNumber = cars[i].assigned_lot;

                // Hvis nummerpladen ikke er assignet til nogen plads
                if (lotNumber == -1) {
                    printf("Car %s is not assigned to any lot.\n", plate);
                    return 0;
                }

                // Print information ift bilen
                for (int j = 0; j < lotCount; j++) {
                    if (lots[j].number == lotNumber) {

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

                // Bilen fundet, men pladsen er ikke fundet (Burde ikke kunne ske)
                printf("Car found but assigned to unknown lot number %d\n", lotNumber);
                return 0;
            }
        }

        //Hvis ikke nogen bil med nummerpladen findes.
        printf("No car with that plate exists.\n");
        return -1;
    }
