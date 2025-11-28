#pragma once
#include "../project/project.h"
#include "raygui.h"
#include <stdbool.h>

#define TOOL_INSPECT 0
#define TOOL_PARKING 1
#define TOOL_ROAD 2
#define TOOL_ENTRANCE 3

static inline Color get_bg_color(void) {
  return GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR));
}

static inline Color get_border_color(void) {
  return GetColor(GuiGetStyle(DEFAULT, BORDER_COLOR_NORMAL));
}

static inline Color get_line_color(void) {
  return GetColor(GuiGetStyle(DEFAULT, LINE_COLOR));
}

void set_window_icon();
void update_screen_size_change(Camera2D *camera, RenderTexture *camera_texture,
                               Rectangle *camera_rect, int *render_width,
                               int *render_height);
bool get_screen_size_change(int *width, int *height);
void draw_centered_text(const char *text, int x, int y, int font_size);
void draw_tab_bar(Project **project);
void draw_tool_bar(int *tool_index, int render_width);
void draw_inspector(int render_width);
void draw_floor_buttons(Project *project);
void draw_grid();
void draw_selection_preview(Camera2D *camera, int render_x, int render_y,
                            int render_width, int render_height);
void handle_inspect_tool(Camera2D *camera, int render_x, int render_y,
                         int tool_index);
void handle_spot_tool(Project *project, const Camera2D *camera, int tool_index,
                      int render_x, int render_y, int render_width,
                      int render_height);
void handle_road_tool(Project *project, const Camera2D *camera, int tool_index,
                      int render_x, int render_y, int render_width,
                      int render_height);
void handle_entrance_tool(Project *project, const Camera2D *camera,
                          int tool_index, int render_x, int render_y,
                          int render_width, int render_height);
void handle_save(Project *project);
bool vector2_equal(Vector2 v1, Vector2 v2);