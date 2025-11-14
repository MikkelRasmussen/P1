#ifndef PARKING_SPOT_H
#define PARKING_SPOT_H

#include "raylib.h"

// Forward declaration, no member access here
typedef struct Project Project;

typedef enum { Default, Handicap, EV, Motorcycle } SpotType;

typedef struct {
    Vector2 position;
    float width;
    float height;
    int spot_id;
    bool occupied;
    int floor;
    char zone;
    SpotType type;
} ParkingSpot;

// Only pointer to Project, no member access
void add_parking_spot(Project *project, Vector2 position, float width, float height, char zone);
void draw_parking_spots(Project *project);

#endif
