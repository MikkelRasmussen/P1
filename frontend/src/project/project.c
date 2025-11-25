#include "project.h"
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

  project->floors = malloc(sizeof(ParkingSpot *));
  if (project->floors == NULL)
    return;
  project->floors[0] = NULL;
}

void free_project(Project **project) {
  if ((*project) == NULL)
    return;

  if ((*project)->path != NULL)
    NFD_FreePathU8((*project)->path);

  // Free each floor's parking spots
  if ((*project)->floors != NULL) {
    for (int i = 0; i < (*project)->floor_count; i++) {
      if ((*project)->floors[i] != NULL) {
        free((*project)->floors[i]);
      }
    }
    free((*project)->floors);
  }

  if (*project != NULL)
    free(*project);

  *project = NULL;
}

void print_project(Project *project) {
  printf("floor_count: %d\n", project->floor_count);
  printf("active_floor: %d\n", project->active_floor);
  printf("spot_counts: [");
  for (int i = 0; i < project->floor_count; i++) {
    printf("%d", project->floors[i]->spot_count);
    if (i >= project->floor_count - 1)
      break;
    printf(", ");
  }
  printf("]\n");
  printf("floors: [\n");
  for (int i = 0; i < project->floor_count; i++) {
    printf("  floor %d: [\n", i);
    for (int j = 0; j < project->floors[i]->spot_count; j++) {
      ParkingSpot spot = project->floors[i]->spots[j];
      printf("    spot %d: {\n", j);
      printf("      position: (%f, %f),\n", spot.position.x, spot.position.y);
      printf("      type: %d,\n", spot.type);
      printf("      zone: %c\n", spot.zone);
      printf("    }\n");
    }
    printf("  ]\n");
  }
  printf("]");
}

void add_floor(Project *project) {
  int new_floor_count = project->floor_count + 1;
  Floor *tmp_floors =
      realloc(project->floors, sizeof(Floor *) * new_floor_count);
  if (tmp_floors == NULL)
    return;
  project->floors[new_floor_count - 1] = NULL;

  project->floor_count = new_floor_count;
  project->active_floor = new_floor_count - 1;
}

void remove_floor(Project *project) {
  int new_floor_count = project->floor_count - 1;
  project->floor_count = new_floor_count;

  if (project->active_floor >= new_floor_count)
    project->active_floor = new_floor_count - 1;
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

  // Read spot counts
  fscanf(file, "%*[^[]"); // Skip everything until '['
  fscanf(file, "%*c");    // Skip the '['
  for (int i = 0; i < (*project)->floor_count; i++) {
    fscanf(
        file, " %d",
        &(*project)->floors[i]->spot_count); // Leading space skips whitespace
    if (i < (*project)->floor_count - 1) {
      fscanf(file, " ,"); // Skip comma with optional whitespace
    }
    fscanf(file, "%*s %d", &(*project)->floors[i]->spot_count);
  }
  fscanf(file, " ]"); // Skip closing bracket

  // Read floors
  fscanf(file, "%*s %*s"); // Skip FLOORS: [
  for (int i = 0; i < (*project)->floor_count; i++) {
    // Allocate memory for this floor's parking spots
    int spot_count = (*project)->floors[i]->spot_count;
    if (spot_count > 0) {
      (*project)->floors[i] = malloc(sizeof(ParkingSpot) * spot_count);
      if ((*project)->floors[i] == NULL) {
        fclose(file);
        free_project(project);
        return;
      }
    } else {
      (*project)->floors[i] = NULL;
    }

    for (int j = 0; j < spot_count; j++) {
      fscanf(file, "%*[^(]"); // Skip everything until '('
      fscanf(file, "%*c");    // Skip the '('

      // Position
      fscanf(file, "%f %*s %f %*s", &(*project)->floors[i]->spots[j].position.x,
             &(*project)->floors[i]->spots[j].position.y);

      // Type
      fscanf(file, "%*s %d %*s", &(*project)->floors[i]->spots[j].type);

      // Zone
      fscanf(file, "%*s %c %*s", &(*project)->floors[i]->spots[j].zone);
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

  // Write floor count
  fprintf(file, "FLOOR_COUNT: %d\n", project->floor_count);

  // Write active floor
  fprintf(file, "ACTIVE_FLOOR: %d\n", project->active_floor);

  // Write spot counts
  fprintf(file, "SPOT_COUNT: [");
  for (int i = 0; i < project->floor_count; i++) {
    fprintf(file, "%d", project->floors[i]->spot_count);
    if (i >= project->floor_count - 1)
      break;
    fprintf(file, ", ");
  }
  fprintf(file, "]\n");

  // Write floors
  fprintf(file, "FLOORS: [\n");
  for (int i = 0; i < project->floor_count; i++) {
    fprintf(file, "  Floor %d: [\n", i);
    for (int j = 0; j < project->floors[i]->spot_count; j++) {
      ParkingSpot spot = project->floors[i]->spots[j];
      fprintf(file, "    Spot %d: {\n", j);
      fprintf(file, "      POSITION: (%f, %f),\n", spot.position.x,
              spot.position.y);
      fprintf(file, "      TYPE: %d,\n", spot.type);
      fprintf(file, "      ZONE: %c\n", spot.zone);
      fprintf(file, "    }");

      if (j < project->floors[i]->spot_count - 1)
        fprintf(file, ",");

      fprintf(file, "\n");
    }
    fprintf(file, "  ]\n");
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
      &project->floors[project->active_floor]->spots;
  int *spot_count = &project->floors[project->active_floor]->spot_count;
  int new_spot_count = *spot_count + 1;

  ParkingSpot *tmp =
      realloc(*active_floor_spots, sizeof(ParkingSpot) * new_spot_count);
  if (tmp == NULL)
    return;

  *active_floor_spots = tmp;
  *spot_count = new_spot_count;
  ParkingSpot *new_spot = &(*active_floor_spots)[new_spot_count - 1];
  new_spot->position = position;
  new_spot->type = Default;
  new_spot->zone = zone;
}

void draw_parking_spots(Project *project) {
  if (project == NULL)
    return;
  ParkingSpot **active_floor = &project->floors[project->active_floor]->spots;
  int *spot_count = &project->floors[project->active_floor]->spot_count;

  for (int i = 0; i < *spot_count; i++) {
    ParkingSpot *spot = &(*active_floor)[i];
    DrawRectangleV(spot->position, (Vector2){50, 50}, RAYWHITE);
  }
}
#pragma endregion