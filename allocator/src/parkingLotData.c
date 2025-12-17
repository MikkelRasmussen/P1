//
// Created by mikke on 14/11/2025.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parkingLot.h"

#define MAX_LINE 128

int lengthOfDataFile()
{
    // Open the file containing parking structere data
    FILE *file = fopen("export.pexport", "r");
    // Print error message if file could not be opened
    if (file == NULL)
    {
        printf("Could not open file data.csv");
        return 1;
    }
    int current_line = 1;
    char c;
    // Read through the file character by character
    do
    {
        // Read next character
        c = fgetc(file);
        // Increment line count on newline character
        if (c == '\n')
            current_line++;
        // Continue until end of file
    } while (c != EOF);
    // Close the file
    fclose(file);

    // Return total line count
    return current_line;
}

int scanDataFile(int id, int *floor, char *zone, int *number, enum type *type, enum status *status) // Status is commented out when not simulating
{
    // Open the data file
    FILE *file = fopen("export.pexport", "r");

    // Print error message if file could not be opened
    if (file == NULL)
    {
        printf("Could not open xporte.pexport\n");
        return 1;
    }
    char line[MAX_LINE];
    int found = 0;

    // Skip header line
    fgets(line, sizeof(line), file);

    // Read through each line of the file
    while (fgets(line, sizeof(line), file))
    {

        char lineCopy[MAX_LINE];
        strcpy(lineCopy, line);
        // Split line by commas to extract fields
        char *parkingID = strtok(lineCopy, ",");
        // Convert ID string to integer for comparison
        int compareID = atoi(parkingID);

        // If the ID matches the requested ID, extract the data
        if (id == compareID)
        {
            // split line again to get all fields
            char *floorStr = strtok(NULL, ",");
            char *zoneStr = strtok(NULL, ",");
            char *numberStr = strtok(NULL, ",");
            char *typeStr = strtok(NULL, ",");
            char *occupancyStr = strtok(NULL, ",");

            // If all fields are present, store the data in the provided pointers
            if (floorStr && zoneStr && numberStr && typeStr)
            {
                // Save extracted data
                *floor = atoi(floorStr);
                strcpy(zone, zoneStr);
                *number = atoi(numberStr);
                *type = atoi(typeStr);
                *status = atoi(occupancyStr);

                found = 1;
                break;
            }
        }
    }

    // If no matching ID was found, print a message
    if (!found)
    {
        printf("No record found for %d\n", id);
    }

    // Close the file
    fclose(file);
    return 0;
}

// Loads car data from a CSV file into an array of car structures
struct car *loadCarsFromCSV(const char *filename, int *carCount)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        printf("Could not open file %s\n", filename);
        *carCount = 0;
        return NULL;
    }

    char line[128];
    int count = 0;

    // Skip header
    fgets(line, sizeof(line), file);

    struct car *cars = NULL;

    while (fgets(line, sizeof(line), file))
    {
        char plate[16];
        char typeStr[16];
        int spot;

        // Read plate, type, and spot
        if (sscanf(line, "%15[^,],%15[^,],%d", plate, typeStr, &spot) != 3)
            continue;

        // Resize array
        struct car *newCars = realloc(cars, (count + 1) * sizeof(struct car));
        if (!newCars)
        {
            printf("Memory allocation failed\n");
            free(cars);
            *carCount = 0;
            fclose(file);
            return NULL;
        }
        cars = newCars;

        strcpy(cars[count].number_plate, plate);

        if (strcmp(typeStr, "STANDARD") == 0)
            cars[count].type = STANDARD;
        else if (strcmp(typeStr, "HANDICAPPED") == 0)
            cars[count].type = HANDICAPPED;
        else if (strcmp(typeStr, "EV") == 0)
            cars[count].type = EV;
        else
            cars[count].type = STANDARD; // default

        // Use spot if valid, otherwise -1
        cars[count].assigned_lot = (spot >= 0) ? spot : -1;

        count++;
    }

    fclose(file);
    *carCount = count;
    return cars;
}