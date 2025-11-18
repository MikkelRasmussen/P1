#include "project.h"
#include "nfd.h"
#include "raygui.h"
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void init_project(Project *project) {
  project->floor_count = 1;  // Default single floor
  project->active_floor = 0; // Start at floor 0
  project->floors = malloc(sizeof(Project *) * project->floor_count);
  project->spot_counts = calloc(project->floor_count, sizeof(int));

  // Initialize zones array to zero and default first zone
  // memset(project->zones, 0, sizeof(project->zones));
  // project->zones[0] = 'A';
}

void free_project(Project **project) {
  if ((*project) == NULL)
    return;

  if ((*project)->path != NULL)
    NFD_FreePathU8((*project)->path);

  if ((*project)->floors != NULL)
    free((*project)->floors);

  if ((*project)->spot_counts != NULL)
    free((*project)->spot_counts);

  if (*project != NULL)
    free(*project);

  *project = NULL;
}

// void set_project_zones(Project *project, const char *zones, int count) {
//   if (project == NULL)
//     return;

//   if (count > MAX_ZONES)
//     count = MAX_ZONES;

//   // Clear existing zones
//   memset(project->zones, 0, sizeof(project->zones));

//   // Copy new zones
//   for (int i = 0; i < count; i++) {
//     project->zones[i] = zones[i];
//   }
// }

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
  init_project(*project);
  (*project)->path = path;

  // Parsing file to project
  // Skipping labels and reading data
  // Read floor count
  fscanf(file, "%*s %d", &(*project)->floor_count);

  // Read active floor
  fscanf(file, "%*s %d", &(*project)->active_floor);

  // Read floors
  fscanf(file, "%*s");

  fclose(file);

  printf("Loaded project: %d floors, active floor: %d\n",
         (*project)->floor_count, (*project)->active_floor);
}

void save_project(Project *project) {
  if (project == NULL) {
    printf("Save: Project is null...\n");
    return;
  }

  // if (!project->has_change) {
  //   printf("Save: No changes to save.\n");
  //   return;
  // }

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
    for (int j = 0; j < project->spot_counts[i]; j++) {
      fprintf(file, "%d, ", project->spot_counts[i]);
    }
  }
  fprintf(file, "]\n");

  // Write floors
  fprintf(file, "FLOORS:\n");

  fclose(file);

  printf("Saved project succesfully!\n");
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
