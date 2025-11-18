#pragma once
#include "raylib.h"

typedef struct Project Project;
typedef enum SpotType { Default, Handicap, EV, Motorcycle } SpotType;

typedef struct ParkingSpot {
  Vector2 position;
  // float width;
  // float height;
  // int spot_id;
  // int floor;
  char zone;
  SpotType type;
} ParkingSpot;

// Only pointer to Project, no member access
void add_parking_spot(Project *project, Vector2 position, char zone);
void draw_parking_spots(Project *project);
