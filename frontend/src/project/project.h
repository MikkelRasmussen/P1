#pragma once
#include "nfd.h"

#define MAX_ZONES 10

typedef struct ParkingSpot ParkingSpot;

// Now define the full struct
typedef struct Project {
  nfdu8char_t *path;
  ParkingSpot **floors;
  int *parking_spot_count;
  int parking_spots_capacity;
  int active_floor;
  int floor_count;
  char zones[MAX_ZONES];
} Project;

// Function declarations
void init_project(Project *project);
void free_project(Project **project);
void set_project_zones(Project *project, const char *zones, int count);
void new_project(Project **project);
void open_project(Project **project);
void save_project(Project *project);
void export_project(Project *project);