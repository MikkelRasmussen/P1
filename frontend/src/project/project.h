#pragma once
#include "nfd.h"
#include <raylib.h>
#include <stdbool.h>

typedef enum SpotType { Default, Handicap, EV, Motorcycle } SpotType;

typedef struct ParkingSpot {
  Vector2 position;
  int id;
  char zone;
  SpotType type;
} ParkingSpot;

typedef struct Road {
  Vector2 position;
  int distance;
} Road;

typedef struct Floor {
  ParkingSpot *spots;
  int spot_count;
  Road *roads;
  int road_count;
  Vector2 *entrances;
  int entrance_count;
} Floor;

// Now define the full struct
typedef struct Project {
  nfdu8char_t *path;
  int floor_count;
  Floor *floors;
  int active_floor;
} Project;

// Function declarations
void init_project(Project *project);
void init_floor(Floor *floor);
void free_project(Project **project);
void add_floor(Project *project);
void remove_floor(Project *project, int floor_index);
void new_project(Project **project);
void open_project(Project **project);
void save_project(Project *project);
void export_project(Project *project);
void print_project(Project *project);

// Parking spot
void add_parking_spot(Project *project, Vector2 position, char zone);
void remove_parking_spot(Project *project, Vector2 position);
void draw_parking_spots(Project *project);

// Roads
void add_road(Project *project, Vector2 position);
void draw_roads(Project *project);

// Entrances
void add_entrance(Project *project, Vector2 position);
void draw_entrances(Project *project);
