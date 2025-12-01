#include "project.h"
#include "../definitions.h"
#include "../utils/utils.h"
#include "nfd.h"
#include "raygui.h"
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Project *project = NULL;

void init_project() {
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

void free_project() {
  if (project == NULL)
    return;

  if (project->path != NULL)
    NFD_FreePathU8(project->path);

  // Free each floor's parking spots
  if (project->floors != NULL) {
    for (int i = 0; i < project->floor_count; i++) {
      Floor *floor = &project->floors[i];

      if (floor->spots != NULL) {
        free(floor->spots);
        floor->spots = NULL;
      }
      if (floor->roads != NULL) {
        free(floor->roads);
        floor->roads = NULL;
      }
      if (floor->entrances != NULL) {
        free(floor->entrances);
        floor->entrances = NULL;
      }
    }

    free(project->floors);
  }

  if (project != NULL)
    free(project);

  project = NULL;
}

void print_project() {
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
    printf("    spot_count: %d\n", floor->spot_count);
    printf("    spots: [\n");
    for (int j = 0; j < floor->spot_count; j++) {
      Spot *spot = &floor->spots[j];
      printf("      spot %d: {\n", j);

      // spot->position
      printf("        position: (%f, %f)\n", spot->position.x,
             spot->position.y);

      // spot->type
      printf("        type: %d\n", spot->type);

      // spot->id
      printf("        id: %d\n", spot->id);

      // spot->zone
      printf("        zone: %c\n", spot->zone);

      printf("      }\n");
    }
    printf("    ]\n");

    // floor->roads
    printf("    road_count: %d\n", floor->road_count);
    printf("    roads: [\n");
    for (int j = 0; j < floor->road_count; j++) {
      Road *road = &floor->roads[j];
      printf("      road %d: {\n", j);

      // road->position
      printf("        position: (%f, %f)\n", road->position.x,
             road->position.y);

      // road->distance
      printf("        distance: %d\n", road->distance);

      printf("      }\n");
    }
    printf("    ]\n");

    // floor->entrances
    printf("    entrance_count: %d\n", floor->entrance_count);
    printf("    entrances: [\n");
    for (int j = 0; j < floor->entrance_count; j++) {
      Vector2 *entrance = &floor->entrances[j];

      // entrance->position
      printf("      entrance %d: (%f, %f)\n", j, entrance->x, entrance->y);
    }
    printf("    ]\n");
    printf("  }\n");
  }
  printf("]\n");
}

void add_floor() {
  int new_floor_count = project->floor_count + 1;
  Floor *tmp_floors = realloc(project->floors, sizeof(Floor) * new_floor_count);
  if (tmp_floors == NULL)
    return;
  project->floors = tmp_floors;
  init_floor(&project->floors[new_floor_count - 1]);

  project->floor_count = new_floor_count;
  project->active_floor = new_floor_count - 1;
}

void remove_floor(int floor_index) {
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
  nfdu8filteritem_t filters[1] = {{"Project Files", "pproject"}};
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
  nfdu8filteritem_t filters[1] = {{"Project Files", "pproject"}};
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

bool get_export_file_path(nfdu8char_t **path) {
  nfdu8filteritem_t filters[1] = {{"Export Files", "pexport"}};
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

void new_project() {
  nfdu8char_t *path;
  if (!get_save_file_path(&path))
    return;

  free_project();

  project = malloc(sizeof(Project));
  init_project();
  project->path = path;

  save_project();
}

void open_project() {
  nfdu8char_t *path;
  if (!get_open_file_path(&path))
    return;

  FILE *file = fopen(path, "r");
  if (file == NULL) {
    NFD_FreePathU8(path);
    return;
  }

  free_project();
  project = malloc(sizeof(Project));
  project->path = path;

  // Parsing file to project
  // Skipping labels and reading data

  // Read floor count
  fscanf(file, "%*s %d", &project->floor_count);

  // Allocate floors array for the actual number of floors
  project->floors = calloc(project->floor_count, sizeof(Floor));
  if (project->floors == NULL) {
    fclose(file);
    free_project();
    return;
  }

  // Read active floor
  fscanf(file, "%*s %d", &project->active_floor);

  // Read floors
  fscanf(file, "%*s %*s"); // Skip "floors: ["
  for (int i = 0; i < project->floor_count; i++) {
    Floor *floor = &project->floors[i];

    fscanf(file, "%*s %*s %*s"); // Skip "floor n: {"
    // Read spot count
    fscanf(file, "%*s %d", &floor->spot_count);

    // Allocate memory for this floor's parking spots
    if (floor->spot_count > 0) {
      floor->spots = malloc(sizeof(Spot) * floor->spot_count);
      if (floor->spots == NULL) {
        fclose(file);
        free_project();
        return;
      }
    } else {
      free(floor->spots);
      floor->spots = NULL;
    }

    for (int j = 0; j < floor->spot_count; j++) {
      fscanf(file, "%*[^(]"); // Skip everything until '('
      fscanf(file, "%*c");    // Skip the '('

      // Position
      fscanf(file, "%f %*s %f %*s", &floor->spots[j].position.x,
             &floor->spots[j].position.y);

      // Type
      fscanf(file, "%*s %d", &floor->spots[j].type);

      // ID
      fscanf(file, "%*s %d", &floor->spots[j].id);

      // Zone
      fscanf(file, "%*s %c %*s", &floor->spots[j].zone);
    }

    fscanf(file, "%*s"); // Skip closing ']'

    // Read road count
    fscanf(file, "%*s %d", &floor->road_count);

    // Allocate memory for this floor's parking spots
    if (floor->road_count > 0) {
      floor->roads = malloc(sizeof(Road) * floor->road_count);
      if (floor->roads == NULL) {
        fclose(file);
        free_project();
        return;
      }
    } else {
      free(floor->roads);
      floor->roads = NULL;
    }

    for (int j = 0; j < floor->road_count; j++) {
      fscanf(file, "%*[^(]"); // Skip everything until '('
      fscanf(file, "%*c");    // Skip the '('

      // Position
      fscanf(file, "%f %*s %f %*s", &floor->roads[j].position.x,
             &floor->roads[j].position.y);

      // Distance
      fscanf(file, "%*s %d %*s", &floor->roads[j].distance);
    }

    fscanf(file, "%*s"); // Skip closing ']'

    // Read entrance count
    fscanf(file, "%*s %d", &floor->entrance_count);

    // Allocate memory for this floor's entrances
    if (floor->entrance_count > 0) {
      floor->entrances = malloc(sizeof(Vector2) * floor->entrance_count);
      if (floor->entrances == NULL) {
        fclose(file);
        free_project();
        return;
      }
    } else {
      free(floor->entrances);
      floor->entrances = NULL;
    }

    for (int j = 0; j < floor->entrance_count; j++) {
      fscanf(file, "%*[^(]"); // Skip everything until '('
      fscanf(file, "%*c");    // Skip the '('

      // Position
      fscanf(file, "%f %*s %f %*s", &floor->entrances[j].x,
             &floor->entrances[j].y);
    }

    fscanf(file, "%*s %*s");
  }

  fclose(file);

  print_project();
}

void save_project() {
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
    fprintf(file, "    spot_count: %d\n", floor->spot_count);
    fprintf(file, "    spots: [\n");
    for (int j = 0; j < floor->spot_count; j++) {
      Spot *spot = &floor->spots[j];
      fprintf(file, "      spot %d: {\n", j);

      // spot->position
      fprintf(file, "        position: (%f, %f)\n", spot->position.x,
              spot->position.y);

      // spot->type
      fprintf(file, "        type: %d\n", spot->type);

      // spot->id
      fprintf(file, "        id: %d\n", spot->id);

      // spot->zone
      fprintf(file, "        zone: %c\n", spot->zone);

      fprintf(file, "      }\n");
    }
    fprintf(file, "    ]\n");

    // floor->roads
    fprintf(file, "    road_count: %d\n", floor->road_count);
    fprintf(file, "    roads: [\n");
    for (int j = 0; j < floor->road_count; j++) {
      Road *road = &floor->roads[j];
      fprintf(file, "      road %d: {\n", j);

      // road->position
      fprintf(file, "        position: (%f, %f)\n", road->position.x,
              road->position.y);

      // road->distance
      fprintf(file, "        distance: %d\n", road->distance);

      fprintf(file, "      }\n");
    }
    fprintf(file, "    ]\n");

    // floor->entrances
    fprintf(file, "    entrance_count: %d\n", floor->entrance_count);
    fprintf(file, "    entrances: [\n");
    for (int j = 0; j < floor->entrance_count; j++) {
      Vector2 *entrance = &floor->entrances[j];

      // entrance->position
      fprintf(file, "      entrance %d: (%f, %f)\n", j, entrance->x,
              entrance->y);
    }
    fprintf(file, "    ]\n");
    fprintf(file, "  }\n");
  }
  fprintf(file, "]");

  fclose(file);

  printf("Saved project succesfully!\n");
  print_project();
}

int spot_comp(const void *a, const void *b) {
  Spot *a_spot = (Spot *)a;
  Spot *b_spot = (Spot *)b;

  Road **a_roads = get_surrounding_roads(a_spot->position);
  Road **b_roads = get_surrounding_roads(b_spot->position);

  int a_min = INT_MAX;
  int b_min = INT_MAX;

  for (int i = 0; i < 4; i++) {
    Road *a_road = a_roads[i];
    Road *b_road = b_roads[i];

    bool a_lower =
        a_road != NULL && a_road->distance != 0 && a_road->distance < a_min;
    bool b_lower =
        b_road != NULL && b_road->distance != 0 && b_road->distance < b_min;
    if (a_lower)
      a_min = a_road->distance;
    if (b_lower)
      b_min = b_road->distance;
  }

  free(a_roads);
  free(b_roads);

  return a_min - b_min;
}

void export_project() {
  if (project == NULL) {
    printf("Export: Project is null...\n");
    return;
  }

  nfdu8char_t *path;
  if (!get_export_file_path(&path))
    return;

  FILE *file = fopen(path, "w");
  NFD_FreePathU8(path);

  if (file == NULL) {
    printf("Error: Could not open file for writing\n");
    return;
  }

  fprintf(file, "<INDEX>,<FLOOR>,<ZONE>,<ID>,<TYPE>\n");
  int count = 1;
  for (int i = 0; i < project->floor_count; i++) {
    Floor *floor = &project->floors[i];
    qsort(floor->spots, floor->spot_count, sizeof(Spot), spot_comp);

    for (int j = 0; j < floor->spot_count; j++) {
      Spot *spot = &floor->spots[j];
      fprintf(file, "%d,%d,%c,%d,%d\n", count, i, spot->zone, spot->id,
              spot->type);
      count++;
    }
  }

  fclose(file);
}

bool is_at(Vector2 position) {
  return is_spot_at(position) || is_road_at(position) ||
         is_entrance_at(position);
}

#pragma region Spot
Spot *get_spot_at(Vector2 position) {
  Floor *active_floor = &project->floors[project->active_floor];

  for (int i = 0; i < active_floor->spot_count; i++) {
    Spot *spot = &active_floor->spots[i];
    if (spot->position.x != position.x || spot->position.y != position.y)
      continue;

    return spot;
  }

  return NULL;
}

bool is_spot_at(Vector2 position) { return get_spot_at(position) != NULL; }

int get_spot_index(Vector2 position) {
  Floor *active_floor = &project->floors[project->active_floor];

  for (int i = 0; i < active_floor->spot_count; i++) {
    Spot *spot = &active_floor->spots[i];
    if (!vector2_equal(spot->position, position))
      continue;

    return i;
  }

  return -1;
}

void add_spot(Vector2 position, char zone) {
  if (project == NULL)
    return;

  Spot **spots = &project->floors[project->active_floor].spots;
  int *spot_count = &project->floors[project->active_floor].spot_count;

  Spot *tmp = realloc(*spots, sizeof(Spot) * (*spot_count + 1));
  if (tmp == NULL)
    return;

  ++*spot_count;
  *spots = tmp;
  Spot *new_spot = &(*spots)[*spot_count - 1];
  new_spot->position = position;
  new_spot->type = Default;
  new_spot->zone = zone;
  new_spot->id = *spot_count;
}

void remove_spot(Vector2 position) {
  if (project == NULL)
    return;

  Spot **spots = &project->floors[project->active_floor].spots;
  int *spot_count = &project->floors[project->active_floor].spot_count;
  int spot_index = get_spot_index(position);
  if (spot_index < 0)
    return;

  if (!vector2_equal(position, position))
    return;

  for (int j = spot_index; j < *spot_count - 1; j++)
    (*spots)[j] = (*spots)[j + 1];

  // Decrease spot count
  --*spot_count;

  Spot *tmp = realloc(*spots, sizeof(Spot) * (*spot_count));
  if (tmp == NULL && *spot_count > 0)
    return;

  *spots = tmp;
}

void draw_spots() {
  if (project == NULL)
    return;

  Spot **active_floor = &project->floors[project->active_floor].spots;
  int *spot_count = &project->floors[project->active_floor].spot_count;

  for (int i = 0; i < *spot_count; i++) {
    Spot *spot = &(*active_floor)[i];
    DrawRectangleV(spot->position, (Vector2){50, 50}, BLUE);
    draw_centered_text(TextFormat("%c%d", spot->zone, spot->id),
                       spot->position.x + 25, spot->position.y + 25, 12);
  }
}
#pragma endregion

#pragma region Roads
Road *get_road_at(Vector2 position) {
  Floor *active_floor = &project->floors[project->active_floor];

  for (int i = 0; i < active_floor->road_count; i++) {
    Road *road = &active_floor->roads[i];
    if (road->position.x != position.x || road->position.y != position.y)
      continue;

    return road;
  }

  return NULL;
}

bool is_road_at(Vector2 position) { return get_road_at(position) != NULL; }

int get_road_index(Vector2 position) {
  Floor *active_floor = &project->floors[project->active_floor];

  for (int i = 0; i < active_floor->road_count; i++) {
    Road *road = &active_floor->roads[i];
    if (!vector2_equal(road->position, position))
      continue;

    return i;
  }

  return -1;
}

bool has_surrounding_entrance(Vector2 position) {
  Vector2 north_pos = (Vector2){position.x, position.y + GRID_SIZE};
  Vector2 south_pos = (Vector2){position.x, position.y - GRID_SIZE};
  Vector2 east_pos = (Vector2){position.x + GRID_SIZE, position.y};
  Vector2 west_pos = (Vector2){position.x - GRID_SIZE, position.y};

  bool at_north = is_entrance_at(north_pos);
  bool at_south = is_entrance_at(south_pos);
  bool at_east = is_entrance_at(east_pos);
  bool at_west = is_entrance_at(west_pos);

  return at_north || at_south || at_east || at_west;
}

Road **get_surrounding_roads(Vector2 position) {
  Vector2 north_pos = (Vector2){position.x, position.y + GRID_SIZE};
  Vector2 south_pos = (Vector2){position.x, position.y - GRID_SIZE};
  Vector2 east_pos = (Vector2){position.x + GRID_SIZE, position.y};
  Vector2 west_pos = (Vector2){position.x - GRID_SIZE, position.y};

  // Allocate array of 4 Road pointers
  Road **roads = malloc(4 * sizeof(Road *));
  if (roads == NULL)
    return NULL;

  roads[0] = get_road_at(north_pos);
  roads[1] = get_road_at(south_pos);
  roads[2] = get_road_at(east_pos);
  roads[3] = get_road_at(west_pos);

  return roads;
}

int get_road_distance(Vector2 position) {
  if (has_surrounding_entrance(position))
    return 1;

  Road **roads = get_surrounding_roads(position);
  if (roads == NULL)
    return 0;

  int min_distance = INT_MAX;
  for (int i = 0; i < 4; i++) {
    Road *road = roads[i];
    if (road == NULL)
      continue;

    if (road->distance >= min_distance)
      continue;

    if (road->distance <= 0)
      continue;

    min_distance = road->distance;
  }

  free(roads);
  return min_distance == INT_MAX ? 0 : min_distance + 1;
}

void update_road(Road *road) {
  int distance = get_road_distance(road->position);
  if (road->distance == distance)
    return;

  road->distance = distance;

  Road **roads = get_surrounding_roads(road->position);
  if (roads == NULL)
    return;

  for (int i = 0; i < 4; i++) {
    Road *neighbor = roads[i];
    if (neighbor == NULL)
      continue;

    update_road(neighbor);
  }

  free(roads);
}

void reset_road(Road *road) {
  if (road->distance == 0)
    return;

  road->distance = 0;

  Road **roads = get_surrounding_roads(road->position);
  if (roads == NULL)
    return;

  for (int i = 0; i < 4; i++) {
    Road *neighbor = roads[i];
    if (neighbor == NULL)
      continue;

    reset_road(neighbor);
  }

  free(roads);
}

void add_road(Vector2 position) {
  if (project == NULL)
    return;

  Road **roads = &project->floors[project->active_floor].roads;
  int *road_count = &project->floors[project->active_floor].road_count;

  Road *tmp = realloc(*roads, sizeof(Road) * (*road_count + 1));
  if (tmp == NULL)
    return;

  ++*road_count;
  *roads = tmp;
  Road *new_road = &(*roads)[*road_count - 1];
  new_road->position = position;
  update_road(new_road);
}

void remove_roads(Vector2 position) {
  if (project == NULL)
    return;

  Road **roads = &project->floors[project->active_floor].roads;
  int *road_count = &project->floors[project->active_floor].road_count;
  int road_index = get_road_index(position);
  if (road_index < 0)
    return;

  if (!vector2_equal(position, position))
    return;

  for (int j = road_index; j < *road_count - 1; j++)
    (*roads)[j] = (*roads)[j + 1];

  // Decrease road count
  --*road_count;

  Road *tmp = realloc(*roads, sizeof(Road) * (*road_count));
  if (tmp == NULL && *road_count > 0)
    return;

  *roads = tmp;
}

void draw_roads() {
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
Vector2 *get_entrance_at(Vector2 position) {
  Floor *active_floor = &project->floors[project->active_floor];

  for (int i = 0; i < active_floor->entrance_count; i++) {
    Vector2 *entrance = &active_floor->entrances[i];
    if (!vector2_equal(*entrance, position))
      continue;

    return entrance;
  }

  return NULL;
}

bool is_entrance_at(Vector2 position) {
  return get_entrance_at(position) != NULL;
}

int get_entrance_index(Vector2 position) {
  Floor *active_floor = &project->floors[project->active_floor];

  for (int i = 0; i < active_floor->entrance_count; i++) {
    Vector2 *entrance = &active_floor->entrances[i];
    if (!vector2_equal(*entrance, position))
      continue;

    return i;
  }

  return -1;
}

void add_entrance(Vector2 position) {
  if (project == NULL)
    return;

  Vector2 **entrances = &project->floors[project->active_floor].entrances;
  int *entrance_count = &project->floors[project->active_floor].entrance_count;

  Vector2 *tmp = realloc(*entrances, sizeof(Vector2) * (*entrance_count + 1));
  if (tmp == NULL)
    return;

  ++*entrance_count;
  *entrances = tmp;
  (*entrances)[*entrance_count - 1] = position;

  Road **roads = get_surrounding_roads(position);
  if (roads == NULL)
    return;

  for (int i = 0; i < 4; i++) {
    Road *road = roads[i];
    if (road == NULL)
      continue;

    update_road(road);
  }

  free(roads);
}

void remove_entrance(Vector2 position) {
  if (project == NULL)
    return;

  Vector2 **entrances = &project->floors[project->active_floor].entrances;
  int *entrance_count = &project->floors[project->active_floor].entrance_count;
  int entrance_index = get_entrance_index(position);
  if (entrance_index < 0)
    return;

  for (int j = entrance_index; j < *entrance_count - 1; j++)
    (*entrances)[j] = (*entrances)[j + 1];

  // Decrease entrance count
  --*entrance_count;

  Vector2 *tmp = realloc(*entrances, sizeof(Vector2) * (*entrance_count));
  if (tmp == NULL && *entrance_count > 0)
    return;

  *entrances = tmp;

  Road **roads = get_surrounding_roads(position);
  if (roads == NULL)
    return;

  for (int i = 0; i < 4; i++) {
    Road *neighbor = roads[i];
    if (neighbor == NULL)
      continue;

    reset_road(neighbor);
  }

  free(roads);

  for (int i = 0; i < *entrance_count; i++) {
    Vector2 *entrance = &(*entrances)[i];

    Road **entrance_roads = get_surrounding_roads(*entrance);
    if (entrance_roads == NULL)
      continue;

    for (int j = 0; j < 4; j++) {
      Road *road = entrance_roads[j];
      if (road == NULL)
        continue;

      update_road(road);
    }

    free(entrance_roads);
  }
}

void draw_entrances() {
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