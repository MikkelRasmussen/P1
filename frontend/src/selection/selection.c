#include "./selection.h"
#include "../camera/camera.h"
#include "../definitions.h"
#include "../project/project.h"
#include "../utils/utils.h"
#include "raylib.h"
#include "selection.h"
#include <math.h>

Selection selection = {0};

void update_selection(Rectangle render_rect)
{
  if (project == NULL)
    return;

  bool left_mouse_pressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
  if (!left_mouse_pressed)
    return;

  bool is_outside_renderer = get_is_outside_renderer(render_rect);
  if (is_outside_renderer)
    return;

  Vector2 grid_pos = get_mouse_grid_pos(render_rect);

  Spot *spot = get_spot_at(grid_pos);
  Road *road = get_road_at(grid_pos);
  Vector2 *entrance = get_entrance_at(grid_pos);

  selection.position = grid_pos;

  if (spot != NULL)
  {
    selection.type = SPOT;
    selection.ptr = spot;
    return;
  }

  if (road != NULL)
  {
    selection.type = ROAD;
    selection.ptr = road;
    return;
  }

  if (entrance != NULL)
  {
    selection.type = ENTRANCE;
    selection.ptr = entrance;
    return;
  }

  selection.type = NONE;
  selection.ptr = NULL;
}

void draw_selection(Rectangle render_rect)
{
  if (selection.type == NONE)
    return;

  DrawRectangleLines(selection.position.x, selection.position.y, GRID_SIZE,
                     GRID_SIZE, RED);
}