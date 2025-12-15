#include "utils.h"
#include "../camera/camera.h"
#include "../definitions.h"
#include "../project/project.h"
#include "../selection/selection.h"
#include "rlgl.h"
#include <math.h>
#include <raylib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void update_screen_size_change(RenderTexture *camera_texture,
                               Rectangle *camera_rect, int *render_width,
                               int *render_height) {
  if (!IsWindowResized())
    return;

  int old_render_width = *render_width;
  int old_render_height = *render_height;
  *render_width = SCREEN_WIDTH - INSPECTOR_WIDTH;
  *render_height = SCREEN_HEIGHT - TAB_BAR_HEIGHT - BUTTON_SIZE;

  UnloadRenderTexture(*camera_texture);

  *camera_texture = LoadRenderTexture(*render_width, *render_height);
  *camera_rect =
      (Rectangle){0.0f, 0.0f, (float)*render_width, (float)-*render_height};

  update_camera_offset(&camera, old_render_width, old_render_height,
                       *render_width, *render_height);
}

void draw_centered_text(const char *text, int x, int y, int font_size) {
  float spacing = 2.0f;
  Vector2 text_size = MeasureTextEx(GetFontDefault(), text, font_size, spacing);
  int centered_x = x - (int)(text_size.x * 0.5f);
  int centered_y = y - (int)(text_size.y * 0.5f);
  DrawTextEx(GetFontDefault(), text, (Vector2){centered_x, centered_y},
             font_size, spacing, LIGHTGRAY);
}

void set_window_icon() {
  Image icon = LoadImage("../icon.png");
  SetWindowIcon(icon);
  UnloadImage(icon);
}

void draw_grid() {
  rlPushMatrix();
  rlTranslatef(0, 25 * 250, 0);
  rlRotatef(90, 1, 0, 0);
  DrawGrid(500, 50);
  rlPopMatrix();
}

void draw_tab_bar() {
  DrawRectangle(0, 0, SCREEN_WIDTH, TAB_BAR_HEIGHT, get_bg_color());
  GuiLine((Rectangle){0, TAB_BAR_HEIGHT, SCREEN_WIDTH, 1}, NULL);

  const char *tooltips[] = {"New Project", "Open Project", "Save Project",
                            "Export Project"};
  const char *icons[] = {NEW_ICON, OPEN_ICON, SAVE_ICON, EXPORT_ICON};
  bool pressed[4] = {false};

  GuiEnableTooltip();
  for (int i = 0; i < 4; i++) {
    GuiSetTooltip(tooltips[i]);
    pressed[i] = GuiButton(
        (Rectangle){.x = TAB_BAR_PADDING + (BUTTON_SIZE + BUTTON_SPACING) * i,
                    .y = TAB_BAR_PADDING,
                    .width = BUTTON_SIZE,
                    .height = BUTTON_SIZE},
        icons[i]);
  }
  GuiDisableTooltip();

  if (pressed[0])
    new_project();
  if (pressed[1])
    open_project();
  if (pressed[2])
    save_project();
  if (pressed[3])
    export_project();

  if (!project)
    return;

  char *path_copy = strdup(project->path);
  if (!path_copy)
    return;

  const char *project_name = path_copy;
  char *last_slash = strrchr(path_copy, '\\');
  char *last_dot = strrchr(path_copy, '.');
  if (last_slash)
    project_name = last_slash + 1;
  if (last_dot)
    *last_dot = '\0';
  const int font_size = 18;
  DrawText(project_name,
           TAB_BAR_PADDING + (BUTTON_SIZE + BUTTON_SPACING) * 4 +
               BUTTON_SIZE / 2,
           TAB_BAR_PADDING + BUTTON_SIZE / 2 - font_size / 2, font_size, WHITE);
  free(path_copy);
}

void draw_tool_bar(int *tool_index, int render_width) {
  GuiLine((Rectangle){0, SCREEN_HEIGHT - BUTTON_SIZE,
                      SCREEN_WIDTH - INSPECTOR_WIDTH, 1},
          NULL);

  GuiToggleGroup((Rectangle){.x = (float)render_width - 4 * BUTTON_SIZE -
                                  3 * BUTTON_SPACING,
                             .y = SCREEN_HEIGHT - BUTTON_SIZE,
                             .width = BUTTON_SIZE,
                             .height = BUTTON_SIZE},
                 TextFormat("%s;%s;%s;%s", INSPECT_ICON, PARKING_ICON,
                            ROAD_ICON, ENTRANCE_ICON),
                 tool_index);
}

void draw_spot_inspector(int render_width) {
  static bool zone_edit = false;
  static bool id_edit = false;
  Spot *spot = (Spot *)selection.ptr;

  // Convert spot->id (int) to string
  char new_zone = spot->zone;
  int new_id = spot->id;
  char zone_str[16] = "";
  char id_str[16] = "";

  sprintf(zone_str, "%c", new_zone);
  if (spot->id >= 0)
    sprintf(id_str, "%d", new_id);
  int half_width = (float)(INSPECTOR_WIDTH - 24) / 2;

  // Zone & ID
  GuiLabel((Rectangle){render_width + 8, TAB_BAR_HEIGHT + 24,
                       INSPECTOR_WIDTH - 16, 32},
           "Zone & ID");

  Rectangle zone_rect = (Rectangle){
      render_width + 8, TAB_BAR_HEIGHT + 24 + 8 + 18, half_width, 32};
  Rectangle id_rect = (Rectangle){render_width + half_width + 16,
                                  TAB_BAR_HEIGHT + 24 + 8 + 18, half_width, 32};

  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    Vector2 mouse_pos = GetMousePosition();
    bool is_inside_zone = CheckCollisionPointRec(mouse_pos, zone_rect);
    bool is_inside_id = CheckCollisionPointRec(mouse_pos, id_rect);

    if (is_inside_zone) {
      zone_edit = true;
      id_edit = false;
    } else if (is_inside_id) {
      zone_edit = false;
      id_edit = true;
    } else {
      zone_edit = false;
      id_edit = false;
    }
  }

  GuiTextBox(zone_rect, zone_str, sizeof(zone_str), zone_edit);
  GuiTextBox(id_rect, id_str, sizeof(id_str), id_edit);

  if (sscanf(zone_str, "%c", &new_zone) == 1) {
    spot->zone = new_zone;
  }

  if (sscanf(id_str, "%d", &new_id) == 1) {
    spot->id = new_id;
  }

  // Type
  GuiLabel((Rectangle){render_width + 8, TAB_BAR_HEIGHT + 24 + 48 + 18,
                       INSPECTOR_WIDTH - 16, 32},
           "Type");
  int type_index = (int)spot->type;
  GuiToggleGroup(
      (Rectangle){render_width + 8, TAB_BAR_HEIGHT + 24 + 48 + 44,
                  (float)(INSPECTOR_WIDTH - 16 - BUTTON_SPACING * 1.5) / 3, 48},
      "Default;Handicap;Electric", &type_index);
  spot->type = (SpotType)type_index;
}

void draw_inspector(int render_width) {
  GuiPanel((Rectangle){render_width, TAB_BAR_HEIGHT,
                       SCREEN_WIDTH - render_width,
                       SCREEN_HEIGHT - TAB_BAR_HEIGHT},
           "Inspector");

  if (selection.type != SPOT)
    return;

  draw_spot_inspector(render_width);
}

void handle_inspect_tool(int tool_index, Rectangle render_rect) {
  if (tool_index != TOOL_INSPECT)
    return;

  bool is_outside_renderer = get_is_outside_renderer(render_rect);
  if (is_outside_renderer)
    return;

  // Dragging the camera
  if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
    Vector2 delta = GetMouseDelta();
    camera.offset.x += delta.x;
    camera.offset.y += delta.y;
  }
}

Vector2 get_grid_pos(Vector2 world_pos) {
  return (Vector2){floor(world_pos.x / GRID_SIZE) * GRID_SIZE,
                   floor(world_pos.y / GRID_SIZE) * GRID_SIZE};
}

Vector2 get_grid_pos_screen(Vector2 screen_pos, Rectangle render_rect) {
  Vector2 render_pos =
      (Vector2){screen_pos.x - render_rect.x, screen_pos.y - render_rect.y};
  Vector2 world_pos = GetScreenToWorld2D(render_pos, camera);

  return get_grid_pos(world_pos);
}

Vector2 get_grid_pos_mouse(Rectangle render_rect) {
  Vector2 mouse_screen_pos = GetMousePosition();
  return get_grid_pos_screen(mouse_screen_pos, render_rect);
}

void handle_spot_tool(int tool_index, Rectangle render_rect) {
  bool should_place =
      tool_index == TOOL_PARKING && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
  if (!should_place)
    return;

  bool is_outside_renderer = get_is_outside_renderer(render_rect);

  Vector2 grid_pos = get_grid_pos_mouse(render_rect);
  bool grid_pos_taken = is_at(grid_pos);
  bool is_spot = is_spot_at(grid_pos);

  if (is_spot) {
    remove_spot(grid_pos);
    return;
  } else if (grid_pos_taken)
    return;

  if (is_outside_renderer)
    return;

  // Assign first zone by default
  add_spot(grid_pos, 'A');
}

void handle_road_tool(int tool_index, Rectangle render_rect) {
  bool should_place =
      tool_index == TOOL_ROAD && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
  if (!should_place)
    return;

  bool is_outside_renderer = get_is_outside_renderer(render_rect);
  if (is_outside_renderer)
    return;

  Vector2 grid_pos = get_grid_pos_mouse(render_rect);
  bool grid_pos_taken = is_at(grid_pos);
  bool is_road = is_road_at(grid_pos);

  if (is_road) {
    remove_roads(grid_pos);
    return;
  } else if (grid_pos_taken)
    return;

  // Assign first zone by default
  add_road(grid_pos);
}

void handle_entrance_tool(int tool_index, Rectangle render_rect) {
  bool should_place =
      tool_index == TOOL_ENTRANCE && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
  if (!should_place)
    return;

  bool is_outside_renderer = get_is_outside_renderer(render_rect);
  if (is_outside_renderer)
    return;

  Vector2 grid_pos = get_grid_pos_mouse(render_rect);
  bool grid_pos_taken = is_at(grid_pos);
  bool is_entrance = is_entrance_at(grid_pos);

  if (is_entrance) {
    remove_entrance(grid_pos);
    return;
  } else if (grid_pos_taken)
    return;

  // Assign first zone by default
  add_entrance(grid_pos);
}

void draw_floor_buttons() {
  static int deleting_floor_index = -1;

  // Handle delete confirmation
  if (deleting_floor_index != -1) {
    int result = GuiMessageBox(
        (Rectangle){(float)(SCREEN_WIDTH - INSPECTOR_WIDTH) / 2 - 125,
                    (float)(SCREEN_HEIGHT - TAB_BAR_HEIGHT - BUTTON_SIZE) / 2 +
                        TAB_BAR_HEIGHT - 50,
                    250, 100},
        "#191#Delete Floor",
        TextFormat("Are you sure you want to delete floor %d?",
                   deleting_floor_index + 1),
        "Confirm;Cancel");
    if (result != -1) {
      if (result == 1)
        remove_floor(deleting_floor_index);
      deleting_floor_index = -1;
    }
  }

  // Draw floor buttons
  for (int i = 0; i < project->floor_count; i++) {
    int x = (BUTTON_SIZE + BUTTON_SPACING) * i;
    Rectangle btn_rect = {x, SCREEN_HEIGHT - BUTTON_SIZE, BUTTON_SIZE,
                          BUTTON_SIZE};
    bool pressed = GuiButton(btn_rect, TextFormat("%d", i + 1));
    bool right_clicked = IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) &&
                         CheckCollisionPointRec(GetMousePosition(), btn_rect);

    if (right_clicked && project->floor_count > 1) {
      deleting_floor_index = i;
      break;
    }
    if (pressed)
      project->active_floor = i;
  }

  // Add floor button
  int x = (BUTTON_SIZE + BUTTON_SPACING) * project->floor_count;
  if (GuiButton(
          (Rectangle){x, SCREEN_HEIGHT - BUTTON_SIZE, BUTTON_SIZE, BUTTON_SIZE},
          "+"))
    add_floor();
}

void handle_save() {
  if (!IsKeyDown(KEY_LEFT_CONTROL))
    return;
  if (!IsKeyPressed(KEY_S))
    return;

  save_project();
}

void draw_selection_preview(Rectangle render_rect) {
  Vector2 mouse = GetMousePosition();
  Rectangle renderArea = {render_rect.x, render_rect.y, render_rect.width,
                          render_rect.height};

  if (!CheckCollisionPointRec(mouse, renderArea))
    return;

  Vector2 grid_pos = get_grid_pos_screen(mouse, render_rect);

  DrawRectangleLines(grid_pos.x, grid_pos.y, GRID_SIZE, GRID_SIZE,
                     ColorAlpha(WHITE, 1.0f));
}

bool vector2_equal(Vector2 v1, Vector2 v2) {
  return v1.x == v2.x && v1.y == v2.y;
}

void update_tool_selection(int *tool_index) {
  int key = GetKeyPressed();
  switch (key) {
  case KEY_ONE:
    *tool_index = TOOL_INSPECT;
    break;
  case KEY_TWO:
    *tool_index = TOOL_PARKING;
    break;
  case KEY_THREE:
    *tool_index = TOOL_ROAD;
    break;
  case KEY_FOUR:
    *tool_index = TOOL_ENTRANCE;
    break;
  }
}