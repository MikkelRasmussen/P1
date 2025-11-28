#pragma once
#include "raygui.h"
#include <raylib.h>
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
void draw_tab_bar();
void draw_tool_bar(int *tool_index, int render_width);
void draw_inspector(int render_width);
void draw_floor_buttons();
void draw_grid();
void draw_selection_preview(Camera2D *camera, Rectangle render_rect);
void handle_inspect_tool(Camera2D *camera, int tool_index,
                         Rectangle render_rect);
void handle_spot_tool(const Camera2D *camera, int tool_index,
                      Rectangle render_rect);
void handle_road_tool(const Camera2D *camera, int tool_index,
                      Rectangle render_rect);
void handle_entrance_tool(const Camera2D *camera, int tool_index,
                          Rectangle render_rect);
void handle_save();
bool vector2_equal(Vector2 v1, Vector2 v2);