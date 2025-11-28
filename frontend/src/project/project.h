#pragma once
#include "nfd.h"
#include <raylib.h>
#include <stdbool.h>

typedef enum SpotType { Default, Handicap, EV, Motorcycle } SpotType;

typedef struct Spot {
  Vector2 position;
  int id;
  char zone;
  SpotType type;
} Spot;

typedef struct Road {
  Vector2 position;
  int distance;
} Road;

typedef struct Floor {
  Spot *spots;
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

extern Project *project;

// Function declarations
void init_project();
void init_floor(Floor *floor);
void free_project();
void add_floor();
void remove_floor(int floor_index);
void new_project();
void open_project();
void save_project();
void export_project();
void print_project();
bool is_at(Vector2 position);

// Parking spot
Spot *get_spot_at(Vector2 position);
bool is_spot_at(Vector2 position);
int spot_comp(const void *a, const void *b);
int get_spot_index(Vector2 position);
void add_spot(Vector2 position, char zone);
void remove_spot(Vector2 position);
void draw_spots(Project *project);

// Roads
Road *get_road_at(Vector2 position);
Road **get_surrounding_roads(Vector2 position);
bool is_road_at(Vector2 position);
void add_road(Vector2 position);
void remove_roads(Vector2 position);
void draw_roads(Project *project);

// Entrances
Vector2 *get_entrance_at(Vector2 position);
bool is_entrance_at(Vector2 position);
int get_entrance_index(Vector2 position);
void add_entrance(Vector2 position);
void remove_entrance(Vector2 position);
void draw_entrances(Project *project);
