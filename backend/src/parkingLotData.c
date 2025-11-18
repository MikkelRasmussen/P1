//
// Created by mikke on 14/11/2025.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parkingLot.h"

#define MAX_LINE 1024

int lengthOfDataFile() {
    //Åben filen data.csv og gem den
    FILE *file = fopen("data.csv", "r");
    //Print hvis der sker en fejl under indlæsningen af filen.
    if (file == NULL) {
        printf("Could not open file data.csv");
        return 1;
    }
    int current_line = 1;
    char c;
    do {
        //Læs den næste character fra filen
        c = fgetc(file);
        // Hvis det er \n er der der en ny linje i filen.
        if (c == '\n') current_line++;
        // Forsæt indtil vi rammer enden af filen
    } while (c != EOF);
    // Lukker filen igen
    fclose(file);

    // Returnere længden af filen
    return current_line;
}

int scanDataFile(int id, int *floor, char *area, int *number) {
    // Åben filen
    FILE *file = fopen("data.csv", "r");
    if (file == NULL) {
        printf("Could not open data.csv\n");
        return 1;
    }
    char line[MAX_LINE];
    int found = 0;

    // Springe over header linjen (Den første linje i filen)
    fgets(line, sizeof(line), file);


    while (fgets(line, sizeof(line), file)) {
        char lineCopy[MAX_LINE];
        strcpy(lineCopy, line);

        char *parkingID = strtok(lineCopy, ",");
        int compareID = atoi(parkingID);

        if (id == compareID){
            char *temp1 = strtok(NULL, ",");
            char *temp2 = strtok(NULL, ",");
            char *temp3 = strtok(NULL, ",");

            if (temp1 && temp2 && temp3) {

                strcpy(area, temp2);
                *number = atoi(temp3);
                *floor = atoi(temp1);

                found = 1;
                break;
            }
        }
    }

    if (!found) {
        printf("No record found for %d\n", id);
    }

    fclose(file);
    return 0;
}