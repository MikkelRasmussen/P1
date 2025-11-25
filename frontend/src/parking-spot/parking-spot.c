#include "parking-spot.h"
#include "../project/project.h"
#include <raylib.h>
#include <stdbool.h>
#include <stdlib.h>

void add_parking_spot(Project *project, Vector2 position, char zone)
{
  if (project == NULL)
    return;
  ParkingSpot **active_floor = &project->floors[project->active_floor];
  int *spot_count = &project->spot_counts[project->active_floor];

  // Check if parking spot at position already exist
  for (int i = 0; i < *spot_count; i++)
  {
    Vector2 pos = (*active_floor)[i].position;
    bool are_positions_equal = pos.x == position.x && pos.y == position.y;
    if (are_positions_equal)
      return;
  }

  int new_spot_count = *spot_count + 1;

  ParkingSpot *tmp =
      realloc(*active_floor, sizeof(ParkingSpot) * new_spot_count);
  if (tmp == NULL)
    return;

  *active_floor = tmp;
  *spot_count = new_spot_count;
  ParkingSpot *new_spot = &(*active_floor)[new_spot_count - 1];
  new_spot->position = position;
  new_spot->type = Default;
  new_spot->zone = zone;
}

void draw_parking_spots(Project *project)
{
  if (project == NULL)
    return;
  ParkingSpot **active_floor = &project->floors[project->active_floor];
  int *spot_count = &project->spot_counts[project->active_floor];

  for (int i = 0; i < *spot_count; i++)
  {
    ParkingSpot *spot = &(*active_floor)[i];
    DrawRectangleV(spot->position, (Vector2){50, 50}, RAYWHITE);
  }
}