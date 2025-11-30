#pragma once

#include <raylib.h>
typedef enum SelectionType { NONE, SPOT, ROAD, ENTRANCE } SelectionType;

typedef struct Selection {
  SelectionType type;
  Vector2 position;
  void *ptr;
} Selection;

extern Selection selection;

void update_selection(Rectangle render_rect);
void draw_selection(Rectangle render_rect);