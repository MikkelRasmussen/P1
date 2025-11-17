#include "project.h"
#include "nfd.h"
#include "raygui.h"
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void init_project(Project *project) {
  project->floors = NULL;
  project->parking_spot_count = NULL;
  project->parking_spots_capacity = 0;

  project->floor_count = 1;  // Default single floor
  project->active_floor = 0; // Start at floor 0

  // Initialize zones array to zero and default first zone
  memset(project->zones, 0, sizeof(project->zones));
  project->zones[0] = 'A';
}

void free_project(Project **project) {
  if ((*project) == NULL)
    return;

  if ((*project)->path != NULL)
    NFD_FreePathU8((*project)->path);

  free(*project);
  *project = NULL;
}

void set_project_zones(Project *project, const char *zones, int count) {
  if (project == NULL)
    return;

  if (count > MAX_ZONES)
    count = MAX_ZONES;

  // Clear existing zones
  memset(project->zones, 0, sizeof(project->zones));

  // Copy new zones
  for (int i = 0; i < count; i++) {
    project->zones[i] = zones[i];
  }
}

bool get_save_file_path(nfdu8char_t **path) {
  nfdu8filteritem_t filters[1] = {{"Project Files", "json"}};
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
  nfdu8filteritem_t filters[1] = {{"Project Files", "json"}};
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
  (*project)->path = path;

  // Create new project file at path
  fclose(fopen(path, "w"));
}

void open_project(Project **project) {
  nfdu8char_t *path;
  if (!get_open_file_path(&path))
    return;

  // Open the file at path in read mode
  FILE *file = fopen(path, "r");

  if (file == NULL) {
    NFD_FreePathU8(path);
    return;
  }

  // Get the file size
  fseek(file, 0, SEEK_END);
  long file_size = ftell(file);
  fseek(file, 0, SEEK_SET);

  char file_contents[file_size + 1];
  fread(file_contents, file_size, 1, file);
  file_contents[file_size] = '\0';
  fclose(file);

  free_project(project);
  *project = malloc(sizeof(Project));
  (*project)->path = path;

  // TODO: Parse file contents
}

void save_project(Project *project) {
  if (project == NULL) {
    printf("Save: Project is null...\n");
    return;
  }

  if (!project->has_change) {
    printf("Save: No changes to save.\n");
    return;
  }

  nfdu8char_t *path = project->path;

  // Open the file at path in write mode
  FILE *file = fopen(path, "w");

  if (file == NULL) {
    NFD_FreePathU8(path);
    return;
  }

  int file_size = 0;
  char file_contents[file_size + 1];
  file_contents[file_size] = '\0';
  fwrite(file_contents, file_size, 1, file);
  fclose(file);
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
