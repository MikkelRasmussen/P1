#include "project.h"
#include "../utils/utils.h"
#include "nfd.h"
#include "raygui.h"
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void init_project(Project *project) {
  project->path = NULL;
  project->floor_count = 1;  // Default single floor
  project->active_floor = 0; // Start at floor 0

  project->floors = malloc(sizeof(Floor));
  if (project->floors == NULL)
    return;

  init_floor(&project->floors[0]);
}

void init_floor(Floor *floor) {
  if (floor == NULL)
    return;

  *floor = (Floor){.spots = NULL,
                   .spot_count = 0,
                   .roads = NULL,
                   .road_count = 0,
                   .entrances = NULL,
                   .entrance_count = 0};
}

void free_project(Project **project) {
  if ((*project) == NULL)
    return;

  if ((*project)->path != NULL)
    NFD_FreePathU8((*project)->path);

  // Free each floor's parking spots
  if ((*project)->floors != NULL) {
    for (int i = 0; i < (*project)->floor_count; i++) {
      Floor *floor = &(*project)->floors[i];

      if (floor->spots != NULL)
        free(floor->spots);

      if (floor->roads != NULL)
        free(floor->roads);
    }

    free((*project)->floors);
  }

  if (*project != NULL)
    free(*project);

  *project = NULL;
}

void print_project(Project *project) {
  // floor_count
  printf("floor_count: %d\n", project->floor_count);

  // active_floor
  printf("active_floor: %d\n", project->active_floor);

  // floors
  printf("floors: [\n");
  for (int i = 0; i < project->floor_count; i++) {
    Floor *floor = &project->floors[i];
    printf("  floor %d: {\n", i);

    // floor->spots
    printf("    spot_count: %d\n", floor[i].spot_count);
    printf("    spots: [\n");
    for (int j = 0; j < floor[i].spot_count; j++) {
      ParkingSpot *spot = &floor[i].spots[j];
      printf("      spot %d: {\n", j);

      // spot->position
      printf("        position: (%f, %f),\n", spot->position.x,
             spot->position.y);

      // spot->type
      printf("        type: %d,\n", spot->type);

      // spot->zone
      printf("        zone: %c\n", spot->zone);

      printf("      }\n");
    }
    printf("    ]\n");

    // floor->roads
    printf("    road_count: %d\n", floor[i].road_count);
    printf("    roads: [\n");
    for (int j = 0; j < floor[i].road_count; j++) {
      Road *road = &floor[i].roads[j];
      printf("      road %d: {\n", j);

      // road->position
      printf("        position: (%f, %f),\n", road->position.x,
             road->position.y);

      // road->distance
      printf("        distance: %d,\n", road->distance);

      printf("      }\n");
    }
    printf("    ]\n");

    // floor->entrances
    printf("    entrance_count: %d\n", floor[i].entrance_count);
    printf("    entrances: [\n");
    for (int j = 0; j < floor[i].entrance_count; j++) {
      Vector2 *entrance = &floor[i].entrances[j];

      // entrance->position
      printf("      road %d: %f %f\n", j, entrance->x, entrance->y);
    }
    printf("    ]\n");
    printf("  }\n");
  }
  printf("]");
}

void add_floor(Project *project) {
  int new_floor_count = project->floor_count + 1;
  Floor *tmp_floors = realloc(project->floors, sizeof(Floor) * new_floor_count);
  if (tmp_floors == NULL)
    return;
  project->floors = tmp_floors;
  init_floor(&project->floors[new_floor_count - 1]);

  project->floor_count = new_floor_count;
  project->active_floor = new_floor_count - 1;
}

void remove_floor(Project *project, int floor_index) {
  project->floor_count--;

  // Shift down elements after floor_index
  for (int i = floor_index; i < project->floor_count; i++)
    project->floors[i] = project->floors[i + 1];

  // Reallocate in respect to new floor count
  Floor *tmp = realloc(project->floors, sizeof(Floor) * project->floor_count);
  if (tmp == NULL) {
    printf("No memory to remove floor!\n");
    return;
  }
  project->floors = tmp;

  // If the removed floor was active, set active floor to last available
  if (project->active_floor >= project->floor_count)
    project->active_floor = project->floor_count - 1;
}

bool get_save_file_path(nfdu8char_t **path) {
  nfdu8filteritem_t filters[1] = {{"Project Files", "parking"}};
  nfdsavedialogu8args_t args = {.filterList = filters, .filterCount = 1};
  nfdresult_t result = NFD_SaveDialogU8_With(path, &args);

  switch (result) {
  case NFD_OKAY:
    printf("Path: %s\n", *path);
    return true;
  case NFD_CANCEL:
    break;
  default:
    printf("Error: %s\n", NFD_GetError());
    break;
  }

  return false;
}

bool get_open_file_path(nfdu8char_t **path) {
  nfdu8filteritem_t filters[1] = {{"Project Files", "parking"}};
  nfdopendialogu8args_t args = {.filterList = filters, .filterCount = 1};
  nfdresult_t result = NFD_OpenDialogU8_With(path, &args);

  switch (result) {
  case NFD_OKAY:
    printf("Path: %s\n", *path);
    return true;
  case NFD_CANCEL:
    break;
  default:
    printf("Error: %s\n", NFD_GetError());
    break;
  }

  return false;
}

void new_project(Project **project) {
  nfdu8char_t *path;
  if (!get_save_file_path(&path))
    return;

  free_project(project);

  (*project) = malloc(sizeof(Project));
  init_project(*project);
  (*project)->path = path;

  save_project(*project);
}

void open_project(Project **project) {
  nfdu8char_t *path;
  if (!get_open_file_path(&path))
    return;

  FILE *file = fopen(path, "r");
  if (file == NULL) {
    NFD_FreePathU8(path);
    return;
  }

  free_project(project);
  *project = malloc(sizeof(Project));
  (*project)->path = path;

  // Parsing file to project
  // Skipping labels and reading data

  // Read floor count
  fscanf(file, "%*s %d", &(*project)->floor_count);

  // Reallocate floors array for the actual number of floors
  (*project)->floors = calloc((*project)->floor_count, sizeof(ParkingSpot *));
  if ((*project)->floors == NULL) {
    fclose(file);
    free_project(project);
    return;
  }

  // Read active floor
  fscanf(file, "%*s %d", &(*project)->active_floor);

  // Read floors
  fscanf(file, "%*s %*s"); // Skip FLOORS: [
  for (int i = 0; i < (*project)->floor_count; i++) {
    // Allocate memory for this floor's parking spots
    int spot_count = (*project)->floors[i].spot_count;
    if (spot_count > 0) {
      (*project)->floors[i].spots = malloc(sizeof(ParkingSpot) * spot_count);
      if ((*project)->floors[i].spots == NULL) {
        fclose(file);
        free_project(project);
        return;
      }
    } else {
      (*project)->floors[i].spots = NULL;
    }

    for (int j = 0; j < spot_count; j++) {
      fscanf(file, "%*[^(]"); // Skip everything until '('
      fscanf(file, "%*c");    // Skip the '('

      // Position
      fscanf(file, "%f %*s %f %*s", &(*project)->floors[i].spots[j].position.x,
             &(*project)->floors[i].spots[j].position.y);

      // Type
      fscanf(file, "%*s %d %*s", &(*project)->floors[i].spots[j].type);

      // Zone
      fscanf(file, "%*s %c %*s", &(*project)->floors[i].spots[j].zone);
    }

    fscanf(file, "%*s"); // Skip closing ']'
  }

  fclose(file);

  print_project(*project);
}

void save_project(Project *project) {
  if (project == NULL) {
    printf("Save: Project is null...\n");
    return;
  }

  nfdu8char_t *path = project->path;

  // Open the file at path in write mode
  FILE *file = fopen(path, "w");
  if (file == NULL) {
    printf("Error: Could not open file for writing\n");
    NFD_FreePathU8(path);
    return;
  }

  // floor_count
  fprintf(file, "floor_count: %d\n", project->floor_count);

  // active_floor
  fprintf(file, "active_floor: %d\n", project->active_floor);

  // floors
  fprintf(file, "floors: [\n");
  for (int i = 0; i < project->floor_count; i++) {
    Floor *floor = &project->floors[i];
    fprintf(file, "  floor %d: {\n", i);

    // floor->spots
    fprintf(file, "    spot_count: %d\n", floor[i].spot_count);
    fprintf(file, "    spots: [\n");
    for (int j = 0; j < floor[i].spot_count; j++) {
      ParkingSpot *spot = &floor[i].spots[j];
      fprintf(file, "      spot %d: {\n", j);

      // spot->position
      fprintf(file, "        position: (%f, %f),\n", spot->position.x,
              spot->position.y);

      // spot->type
      fprintf(file, "        type: %d,\n", spot->type);

      // spot->zone
      fprintf(file, "        zone: %c\n", spot->zone);

      fprintf(file, "      }\n");
    }
    fprintf(file, "    ]\n");

    // floor->roads
    fprintf(file, "    road_count: %d\n", floor[i].road_count);
    fprintf(file, "    roads: [\n");
    for (int j = 0; j < floor[i].road_count; j++) {
      Road *road = &floor[i].roads[j];
      fprintf(file, "      road %d: {\n", j);

      // road->position
      fprintf(file, "        position: (%f, %f),\n", road->position.x,
              road->position.y);

      // road->distance
      fprintf(file, "        distance: %d,\n", road->distance);

      fprintf(file, "      }\n");
    }
    fprintf(file, "    ]\n");

    // floor->entrances
    fprintf(file, "    entrance_count: %d\n", floor[i].entrance_count);
    fprintf(file, "    entrances: [\n");
    for (int j = 0; j < floor[i].entrance_count; j++) {
      Vector2 *entrance = &floor[i].entrances[j];

      // entrance->position
      fprintf(file, "      road %d: %f %f\n", j, entrance->x, entrance->y);
    }
    fprintf(file, "    ]\n");
    fprintf(file, "  }\n");
  }
  fprintf(file, "]");

  fclose(file);

  printf("Saved project succesfully!\n");
  print_project(project);
}

void export_project(Project *project) {
  if (project == NULL) {
    printf("Export: Project is null...\n");
    return;
  }

  nfdu8char_t *path;
  if (!get_save_file_path(&path))
    return;

  NFD_FreePathU8(path);
  // TODO: Export project to path
}

#pragma region Parking Spot
void add_parking_spot(Project *project, Vector2 position, char zone) {
  if (project == NULL)
    return;

  ParkingSpot **active_floor_spots =
      &project->floors[project->active_floor].spots;
  int *spot_count = &project->floors[project->active_floor].spot_count;
  ++*spot_count;

  ParkingSpot *tmp =
      realloc(*active_floor_spots, sizeof(ParkingSpot) * *spot_count);
  if (tmp == NULL)
    return;

  *active_floor_spots = tmp;
  ParkingSpot *new_spot = &(*active_floor_spots)[*spot_count - 1];
  new_spot->position = position;
  new_spot->type = Default;
  new_spot->zone = zone;
  new_spot->id = *spot_count;
}

void draw_parking_spots(Project *project) {
  if (project == NULL)
    return;

  ParkingSpot **active_floor = &project->floors[project->active_floor].spots;
  int *spot_count = &project->floors[project->active_floor].spot_count;

  for (int i = 0; i < *spot_count; i++) {
    ParkingSpot *spot = &(*active_floor)[i];
    DrawRectangleV(spot->position, (Vector2){50, 50}, BLUE);
    draw_centered_text(TextFormat("%c%d", spot->zone, spot->id),
                       spot->position.x + 25, spot->position.y + 25, 12);
  }
}
#pragma endregion

#pragma region Roads
void add_road(Project *project, Vector2 position) {
  if (project == NULL)
    return;

  Road **active_floor_roads = &project->floors[project->active_floor].roads;
  int *road_count = &project->floors[project->active_floor].road_count;
  ++*road_count;

  Road *tmp = realloc(*active_floor_roads, sizeof(Road) * *road_count);
  if (tmp == NULL)
    return;

  *active_floor_roads = tmp;
  Road *new_spot = &(*active_floor_roads)[*road_count - 1];
  new_spot->position = position;
  new_spot->distance = 0;
}

void draw_roads(Project *project) {
  if (project == NULL)
    return;

  Road **roads = &project->floors[project->active_floor].roads;
  int *road_count = &project->floors[project->active_floor].road_count;

  for (int i = 0; i < *road_count; i++) {
    Road *road = &(*roads)[i];
    DrawRectangleV(road->position, (Vector2){50, 50}, BLACK);
    draw_centered_text(TextFormat("%d", road->distance), road->position.x + 25,
                       road->position.y + 25, 12);
  }
}
#pragma endregion

#pragma region Entrances
void add_entrance(Project *project, Vector2 position) {
  if (project == NULL)
    return;

  Vector2 **active_floor_entrances =
      &project->floors[project->active_floor].entrances;
  int *entrance_count = &project->floors[project->active_floor].entrance_count;
  ++*entrance_count;

  Vector2 *tmp =
      realloc(*active_floor_entrances, sizeof(Vector2) * *entrance_count);
  if (tmp == NULL)
    return;

  *active_floor_entrances = tmp;
  Vector2 *new_spot = &(*active_floor_entrances)[*entrance_count - 1];
  *new_spot = position;
}

void draw_entrances(Project *project) {
  if (project == NULL)
    return;

  Vector2 **entrances = &project->floors[project->active_floor].entrances;
  int *entrance_count = &project->floors[project->active_floor].entrance_count;

  for (int i = 0; i < *entrance_count; i++) {
    Vector2 *entrance = &(*entrances)[i];
    DrawRectangleV(*entrance, (Vector2){50, 50}, GREEN);
  }
}
#pragma endregion