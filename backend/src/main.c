#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parkingLot.h"


int main() {
    int id = 3;
    int fileLength = lengthOfDataFile();
    printf("File Length: %d\n", fileLength);
    scanDataFile(id);
}