#include "parking-spot.h"
#include "../project/project.h"
#include <stdlib.h>

void add_parking_spot(Project *project, Vector2 position, float width,
                      float height, char zone) {
  if (project->parking_spots_capacity == 0) {
    project->parking_spots_capacity = 10;
    // project->parking_spots = (ParkingSpot*)malloc(sizeof(ParkingSpot) *
    // project->parking_spots_capacity); if (!project->parking_spots) return;
  }

  if (project->spot_counts[project->active_floor] >=
      project->parking_spots_capacity) {
    project->parking_spots_capacity *= 2;
    ParkingSpot *new_array = (ParkingSpot *)realloc(
        project->floors[project->active_floor],
        sizeof(ParkingSpot) * project->parking_spots_capacity);
    if (!new_array)
      return;
    project->floors[project->active_floor] = new_array;
  }

  ParkingSpot spot = {.position = position,
                      .width = width,
                      .height = height,
                      .spot_id =
                          project->spot_counts[project->active_floor] + 1,
                      .occupied = false,
                      .floor = project->active_floor,
                      .zone = zone,
                      .type = Default};

  int current_count = project->spot_counts[project->active_floor];
  project->floors[project->active_floor][current_count] = spot;
  project->spot_counts[project->active_floor]++;
}

void draw_parking_spots(Project *project) {}