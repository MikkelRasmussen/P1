#pragma once
#include "../car/car.h"

typedef enum { Default, Handicap, EV } SpotType;

typedef struct {
  int floor;
  char zone;
  int spot;
  SpotType type;
  Car car;
} ParkingSpot;