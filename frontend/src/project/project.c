#include "project.h"
#include "nfd.h"
#include "raygui.h"
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>

void free_project(Project **project) {
  if ((*project) == NULL)
    return;

  if ((*project)->path != NULL)
    NFD_FreePathU8((*project)->path);

  free(*project);
  *project = NULL;
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
  (*project) = malloc(sizeof(Project));
  (*project)->path = path;

  // TODO: Parse file contents
}

void save_project(Project *project) {
  if (project == NULL)
    return;

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
}

void export_project(Project *project) {
  if (project == NULL)
    return;

  nfdu8char_t *path;
  if (!get_save_file_path(&path))
    return;

  NFD_FreePathU8(path);
  // TODO: Export project to path
}