#include "utils.h"
#include "../camera/camera.h"
#include "../definitions.h"
#include "rlgl.h"
#include <raylib.h>
#include <string.h>

void update_screen_size_change(Camera2D *camera, RenderTexture *camera_texture,
                               Rectangle *camera_rect, int *screen_width,
                               int *screen_height, int *render_width,
                               int *render_height) {
  int did_screen_size_change =
      get_screen_size_change(screen_width, screen_height);

  if (did_screen_size_change) {
    int old_render_width = *render_width;
    int old_render_height = *render_height;
    *render_width = *screen_width - INSPECTOR_WIDTH;
    *render_height = *screen_height - TOOL_BAR_HEIGHT;

    UnloadRenderTexture(*camera_texture);

    *camera_texture = LoadRenderTexture(*render_width, *render_height);
    *camera_rect =
        (Rectangle){0.0f, 0.0f, (float)*render_width, (float)-*render_height};

    update_camera_offset(camera, old_render_width, old_render_height,
                         *render_width, *render_height);
  }
}

int get_screen_size_change(int *width, int *height) {
  int new_width = GetScreenWidth();
  int new_height = GetScreenHeight();

  int did_change = *width != new_width || *height != new_height;
  if (!did_change)
    return 0;

  *width = new_width;
  *height = new_height;
  return 1;
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

void draw_tab_bar(int width) {
  DrawRectangle(0, 0, width, TOOL_BAR_HEIGHT, get_bg_color());
  GuiLine((Rectangle){0, TOOL_BAR_HEIGHT, width, 1}, NULL);

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

void draw_tool_bar(int *tool_index, int render_width, int screen_height) {
  GuiToggleGroup((Rectangle){.x = (float)render_width / 2 - 1.5f * BUTTON_SIZE,
                             .y = screen_height - BUTTON_SIZE,
                             .width = BUTTON_SIZE,
                             .height = BUTTON_SIZE},
                 "#220#;#221#;#222#", tool_index);
}

void draw_inspector(int render_width, int screen_width, int screen_height) {
  GuiPanel((Rectangle){render_width, TOOL_BAR_HEIGHT,
                       screen_width - render_width,
                       screen_height - TOOL_BAR_HEIGHT},
           "Inspector");
}

void draw_floor_buttons(int *floors, int screen_height) {
  // Calculate the button container total height and the y-coord of the
  // container
  int total_height = (*floors + 1) * (BUTTON_SIZE) + BUTTON_SPACING * *floors;
  int base_y =
      ((screen_height - TOOL_BAR_HEIGHT) - total_height) / 2 + TOOL_BAR_HEIGHT;

  // Draw floor buttons
  for (int i = 0; i < *floors; i++) {
    int y = base_y + (BUTTON_SIZE + BUTTON_SPACING) * i;
    GuiButton((Rectangle){0, y, BUTTON_SIZE, BUTTON_SIZE},
              TextFormat("%d", i + 1));
  }

  // Draw add floor button
  int y = base_y + (BUTTON_SIZE + BUTTON_SPACING) * *floors;
  int add_floor = GuiButton((Rectangle){0, y, BUTTON_SIZE, BUTTON_SIZE}, "+");

  // Add floor, if button was pressed
  if (add_floor)
    (*floors)++;
}