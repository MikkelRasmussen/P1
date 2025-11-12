#pragma once
#include "../parking-spot/parking-spot.h"

typedef struct {
  ParkingSpot *parking_spots;
  int floors;
  char zones[26];
} ParkingLot;