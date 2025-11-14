#ifndef PROJECT_H
#define PROJECT_H

#include "../parking-spot/parking-spot.h"
#include "nfd.h"

#define MAX_ZONES 10

typedef struct
{
  ParkingSpot *parking_spots;
  nfdu8char_t *path;
  ParkingSpot **floors;
  int parking_spots_count;
  int parking_spots_capacity;
  int *parking_spot_count;
  int active_floor;
  int floor_count;
  char zones[MAX_ZONES];
} Project;

// Initialize a project with default values
void init_project(Project *project);

// Free all dynamically allocated memory
void free_project(Project *project);

// Set project zones
void set_project_zones(Project *project, const char *zones, int count);

#endif // PROJECT_H

void new_project(Project **project);
void open_project(Project **project);
void save_project(Project *project);
void export_project(Project *project);