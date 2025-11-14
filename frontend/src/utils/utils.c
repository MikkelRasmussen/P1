#include "utils.h"
#include "../camera/camera.h"
#include "../definitions.h"
#include "../project/project.h"
#include "rlgl.h"
#include <math.h>
#include <raylib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

void update_screen_size_change(Camera2D *camera, RenderTexture *camera_texture,
                               Rectangle *camera_rect, int *render_width,
                               int *render_height) {

  if (!IsWindowResized())
    return;

  int old_render_width = *render_width;
  int old_render_height = *render_height;
  *render_width = SCREEN_WIDTH - INSPECTOR_WIDTH;
  *render_height = SCREEN_HEIGHT - TOOL_BAR_HEIGHT;

  UnloadRenderTexture(*camera_texture);

  *camera_texture = LoadRenderTexture(*render_width, *render_height);
  *camera_rect =
      (Rectangle){0.0f, 0.0f, (float)*render_width, (float)-*render_height};

  update_camera_offset(camera, old_render_width, old_render_height,
                       *render_width, *render_height);
}

void draw_centered_text(char *text, int x, int y, int font_size) {
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
  DrawRectangle(0, 0, SCREEN_WIDTH, TOOL_BAR_HEIGHT, get_bg_color());
  GuiLine((Rectangle){0, TOOL_BAR_HEIGHT, SCREEN_WIDTH, 1}, NULL);

  GuiEnableTooltip();
  // New
  GuiSetTooltip("New Project");
  GuiButton(
      (Rectangle){.x = TOOL_BAR_PADDING + (BUTTON_SIZE + BUTTON_SPACING) * 0,
                  .y = TOOL_BAR_PADDING,
                  .width = BUTTON_SIZE,
                  .height = BUTTON_SIZE},
      NEW_ICON);
  // Open
  GuiSetTooltip("Open Project");
  GuiButton(
      (Rectangle){.x = TOOL_BAR_PADDING + (BUTTON_SIZE + BUTTON_SPACING) * 1,
                  .y = TOOL_BAR_PADDING,
                  .width = BUTTON_SIZE,
                  .height = BUTTON_SIZE},
      OPEN_ICON);
  // Save
  GuiSetTooltip("Save Project");
  GuiButton(
      (Rectangle){.x = TOOL_BAR_PADDING + (BUTTON_SIZE + BUTTON_SPACING) * 2,
                  .y = TOOL_BAR_PADDING,
                  .width = BUTTON_SIZE,
                  .height = BUTTON_SIZE},
      SAVE_ICON);
  // Export
  GuiSetTooltip("Export Project");
  GuiButton(
      (Rectangle){.x = TOOL_BAR_PADDING + (BUTTON_SIZE + BUTTON_SPACING) * 3,
                  .y = TOOL_BAR_PADDING,
                  .width = BUTTON_SIZE,
                  .height = BUTTON_SIZE},
      EXPORT_ICON);
  GuiDisableTooltip();
}

void draw_tool_bar(int *tool_index, int render_width) {
  GuiToggleGroup((Rectangle){.x = (float)render_width / 2 - 2 * BUTTON_SIZE -
                                  1.5f * BUTTON_SPACING,
                             .y = SCREEN_HEIGHT - BUTTON_SIZE,
                             .width = BUTTON_SIZE,
                             .height = BUTTON_SIZE},
                 TextFormat("%s;%s;%s;%s", INSPECT_ICON, PARKING_ICON,
                            ROAD_ICON, ENTRANCE_ICON),
                 tool_index);
}

void draw_inspector(int render_width) {
  GuiPanel((Rectangle){render_width, TOOL_BAR_HEIGHT,
                       SCREEN_WIDTH - render_width,
                       SCREEN_HEIGHT - TOOL_BAR_HEIGHT},
           "Inspector");
}

int deleting_floor_index = -1;
void draw_floor_buttons(Project *project) {
  if (deleting_floor_index != -1) {
    int message_result =
        GuiMessageBox((Rectangle){(float)SCREEN_WIDTH / 2 - 125,
                                  (float)SCREEN_HEIGHT / 2 - 50, 250, 100},
                      "#191#Delete Floor",
                      TextFormat("Are you sure you want to delete floor %d?",
                                 deleting_floor_index + 1),
                      "Confirm;Cancel");
    if (message_result != -1) {
      if (message_result == 1)
        project->floors--;

      deleting_floor_index = -1;
    }
  }

  // Calculate the button container total height and the y-coord of the
  // container
  int total_height =
      (project->floors + 1) * (BUTTON_SIZE) + BUTTON_SPACING * project->floors;
  int base_y =
      ((SCREEN_HEIGHT - TOOL_BAR_HEIGHT) - total_height) / 2 + TOOL_BAR_HEIGHT;

  // Draw floor buttons
  for (int i = 0; i < project->floors; i++) {
    int y = base_y + (BUTTON_SIZE + BUTTON_SPACING) * i;
    Rectangle btn_rect = (Rectangle){0, y, BUTTON_SIZE, BUTTON_SIZE};
    int is_pressed = GuiButton(btn_rect, TextFormat("%d", i + 1));
    int was_right_clicked =
        IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) &&
        CheckCollisionPointRec(GetMousePosition(), btn_rect);

    if (was_right_clicked) {
      deleting_floor_index = i;
      return;
    }

    if (!is_pressed)
      continue;

    project->active_floor = i;
  }

  // Draw add floor button
  int y = base_y + (BUTTON_SIZE + BUTTON_SPACING) * project->floors;
  int add_floor = GuiButton((Rectangle){0, y, BUTTON_SIZE, BUTTON_SIZE}, "+");

  // Add floor, if button was pressed
  if (add_floor)
    project->floors++;
}