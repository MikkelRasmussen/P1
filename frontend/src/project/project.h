#pragma once
#include "nfd.h"
#include <stdbool.h>

typedef struct ParkingSpot ParkingSpot;

// Now define the full struct
typedef struct Project {
  nfdu8char_t *path;
  int floor_count;
  int *spot_count;
  ParkingSpot **floors;
  int parking_spots_capacity;
  int active_floor;
  bool has_change;
} Project;

// Function declarations
void init_project(Project *project);
void free_project(Project **project);
// void set_project_zones(Project *project, const char *zones, int count);
void new_project(Project **project);
void open_project(Project **project);
void save_project(Project *project);
void export_project(Project *project);