#pragma once
#include "../parking-spot/parking-spot.h"

typedef struct {
  ParkingSpot *parking_spots;
  int floors;
  int active_floor;
  char zones[26];
} Project;

void save_project(Project *project);
Project load_project();