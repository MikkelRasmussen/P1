#include "utils.h"
#include "../camera/camera.h"
#include "../definitions.h"
#include "../project/project.h"
#include "rlgl.h"
#include <math.h>
#include <raylib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void update_screen_size_change(Camera2D *camera, RenderTexture *camera_texture,
                               Rectangle *camera_rect, int *render_width,
                               int *render_height) {
  if (!IsWindowResized())
    return;

  int old_render_width = *render_width;
  int old_render_height = *render_height;
  *render_width = SCREEN_WIDTH - INSPECTOR_WIDTH;
  *render_height = SCREEN_HEIGHT - TAB_BAR_HEIGHT;

  UnloadRenderTexture(*camera_texture);

  *camera_texture = LoadRenderTexture(*render_width, *render_height);
  *camera_rect =
      (Rectangle){0.0f, 0.0f, (float)*render_width, (float)-*render_height};

  update_camera_offset(camera, old_render_width, old_render_height,
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

  GuiEnableTooltip();
  // New
  GuiSetTooltip("New Project");
  bool is_new_pressed = GuiButton(
      (Rectangle){.x = TAB_BAR_PADDING + (BUTTON_SIZE + BUTTON_SPACING) * 0,
                  .y = TAB_BAR_PADDING,
                  .width = BUTTON_SIZE,
                  .height = BUTTON_SIZE},
      NEW_ICON);
  // Open
  GuiSetTooltip("Open Project");
  bool is_open_pressed = GuiButton(
      (Rectangle){.x = TAB_BAR_PADDING + (BUTTON_SIZE + BUTTON_SPACING) * 1,
                  .y = TAB_BAR_PADDING,
                  .width = BUTTON_SIZE,
                  .height = BUTTON_SIZE},
      OPEN_ICON);
  // Save
  GuiSetTooltip("Save Project");
  bool is_save_pressed = GuiButton(
      (Rectangle){.x = TAB_BAR_PADDING + (BUTTON_SIZE + BUTTON_SPACING) * 2,
                  .y = TAB_BAR_PADDING,
                  .width = BUTTON_SIZE,
                  .height = BUTTON_SIZE},
      SAVE_ICON);
  // Export
  GuiSetTooltip("Export Project");
  bool is_export_pressed = GuiButton(
      (Rectangle){.x = TAB_BAR_PADDING + (BUTTON_SIZE + BUTTON_SPACING) * 3,
                  .y = TAB_BAR_PADDING,
                  .width = BUTTON_SIZE,
                  .height = BUTTON_SIZE},
      EXPORT_ICON);
  GuiDisableTooltip();

  if (project != NULL) {
    char *path_copy = strdup(project->path);
    if (path_copy == NULL)
      return;

    const char *project_name = path_copy;
    const char *last_slash = strrchr(path_copy, '\\');
    char *last_dot = strrchr(path_copy, '.');

    if (last_slash != NULL)
      project_name = last_slash + 1;

    if (last_dot != NULL)
      *last_dot = '\0';

    const int font_size = 18;
    DrawText(
        project_name,
        TAB_BAR_PADDING + (BUTTON_SIZE + BUTTON_SPACING) * 4 + BUTTON_SIZE / 2,
        TAB_BAR_PADDING + BUTTON_SIZE / 2 - font_size / 2, font_size, WHITE);

    free(path_copy);
  }

  if (is_new_pressed)
    new_project();

  if (is_open_pressed)
    open_project();

  if (is_save_pressed)
    save_project();

  if (is_export_pressed)
    export_project();
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

void draw_inspector(int render_width) {
  GuiPanel((Rectangle){render_width, TAB_BAR_HEIGHT,
                       SCREEN_WIDTH - render_width,
                       SCREEN_HEIGHT - TAB_BAR_HEIGHT},
           "Inspector");
}

void handle_inspect_tool(Camera2D *camera, int tool_index,
                         Rectangle render_rect) {
  if (tool_index != TOOL_INSPECT)
    return;

  // Dragging the camera
  if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
    Vector2 delta = GetMouseDelta();
    camera->offset.x += delta.x;
    camera->offset.y += delta.y;
  }
}

Vector2 get_mouse_grid_pos(const Camera2D *camera, Rectangle render_rect) {
  Vector2 mouse_screen_pos = GetMousePosition();
  Vector2 mouse_render_pos = (Vector2){mouse_screen_pos.x - render_rect.x,
                                       mouse_screen_pos.y - render_rect.y};

  Vector2 mouse_world_pos = GetScreenToWorld2D(mouse_render_pos, *camera);

  // Snap to grid
  return (Vector2){floor(mouse_world_pos.x / GRID_SIZE) * GRID_SIZE,
                   floor(mouse_world_pos.y / GRID_SIZE) * GRID_SIZE};
}

void handle_spot_tool(const Camera2D *camera, int tool_index,
                      Rectangle render_rect) {
  bool should_place =
      tool_index == TOOL_PARKING && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
  if (!should_place)
    return;

  bool is_outside_renderer = get_is_outside_renderer(render_rect);

  Vector2 grid_pos = get_mouse_grid_pos(camera, render_rect);
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

void handle_road_tool(const Camera2D *camera, int tool_index,
                      Rectangle render_rect) {
  bool should_place =
      tool_index == TOOL_ROAD && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
  if (!should_place)
    return;

  bool is_outside_renderer = get_is_outside_renderer(render_rect);
  if (is_outside_renderer)
    return;

  Vector2 grid_pos = get_mouse_grid_pos(camera, render_rect);
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

void handle_entrance_tool(const Camera2D *camera, int tool_index,
                          Rectangle render_rect) {
  bool should_place =
      tool_index == TOOL_ENTRANCE && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
  if (!should_place)
    return;

  bool is_outside_renderer = get_is_outside_renderer(render_rect);
  if (is_outside_renderer)
    return;

  Vector2 grid_pos = get_mouse_grid_pos(camera, render_rect);
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

int deleting_floor_index = -1;
void draw_floor_buttons() {
  if (deleting_floor_index != -1) {
    int message_result = GuiMessageBox(
        (Rectangle){(float)(SCREEN_WIDTH - INSPECTOR_WIDTH) / 2 - 125,
                    (float)(SCREEN_HEIGHT - TAB_BAR_HEIGHT - BUTTON_SIZE) / 2 +
                        TAB_BAR_HEIGHT - 50,
                    250, 100},
        "#191#Delete Floor",
        TextFormat("Are you sure you want to delete floor %d?",
                   deleting_floor_index + 1),
        "Confirm;Cancel");
    if (message_result != -1) {
      if (message_result == 1)
        remove_floor(deleting_floor_index);

      deleting_floor_index = -1;
    }
  }

  // Calculate the button container total height and the y-coord of the
  // container
  int total_width = (project->floor_count + 1) * BUTTON_SIZE +
                    BUTTON_SPACING * project->floor_count;
  // int base_y =
  //     ((SCREEN_HEIGHT - TAB_BAR_HEIGHT) - total_width) / 2 + TAB_BAR_HEIGHT;

  // Draw floor buttons
  for (int i = 0; i < project->floor_count; i++) {
    int x = (BUTTON_SIZE + BUTTON_SPACING) * i;
    Rectangle btn_rect =
        (Rectangle){x, SCREEN_HEIGHT - BUTTON_SIZE, BUTTON_SIZE, BUTTON_SIZE};
    int is_pressed = GuiButton(btn_rect, TextFormat("%d", i + 1));
    int was_right_clicked =
        IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) &&
        CheckCollisionPointRec(GetMousePosition(), btn_rect);

    if (was_right_clicked && project->floor_count > 1) {
      deleting_floor_index = i;
      break;
    }

    if (!is_pressed)
      continue;

    project->active_floor = i;
  }

  // Draw add floor button
  int x = (BUTTON_SIZE + BUTTON_SPACING) * project->floor_count;
  int should_add_floor = GuiButton(
      (Rectangle){x, SCREEN_HEIGHT - BUTTON_SIZE, BUTTON_SIZE, BUTTON_SIZE},
      "+");

  // Add floor, if button was pressed
  if (should_add_floor)
    add_floor();
}

void handle_save() {
  if (!IsKeyDown(KEY_LEFT_CONTROL))
    return;
  if (!IsKeyPressed(KEY_S))
    return;

  save_project();
}

void draw_selection_preview(Camera2D *camera, Rectangle render_rect) {
  Vector2 mouse = GetMousePosition();
  Rectangle renderArea = {render_rect.x, render_rect.y, render_rect.width,
                          render_rect.height};

  if (!CheckCollisionPointRec(mouse, renderArea))
    return;

  Vector2 local = {mouse.x - render_rect.x, mouse.y - render_rect.y};

  Vector2 world = GetScreenToWorld2D(local, *camera);

  Vector2 snapped = {floorf(world.x / GRID_SIZE) * GRID_SIZE,
                     floorf(world.y / GRID_SIZE) * GRID_SIZE};

  DrawRectangleLines(snapped.x, snapped.y, GRID_SIZE, GRID_SIZE * 1.0f,
                     ColorAlpha(WHITE, 1.0f));
}

bool vector2_equal(Vector2 v1, Vector2 v2) {
  return v1.x == v2.x && v1.y == v2.y;
}
