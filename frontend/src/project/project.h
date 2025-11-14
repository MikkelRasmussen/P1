#pragma once
#include "../parking-spot/parking-spot.h"
#include "nfd.h"

typedef struct {
  nfdu8char_t *path;
  ParkingSpot **floors;
  int *parking_spot_count;
  int floor_count;
  int active_floor;
  char zones[26];
} Project;

void new_project(Project **project);
void open_project(Project **project);
void save_project(Project *project);
void export_project(Project *project);