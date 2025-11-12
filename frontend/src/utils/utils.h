#pragma once
#include "raygui.h"

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
                               Rectangle *camera_rect, int *screen_width,
                               int *screen_height, int *render_width,
                               int *render_height);
int get_screen_size_change(int *width, int *height);
void draw_centered_text(char *text, int x, int y, int font_size);
void draw_tool_bar(int width);
void draw_floor_buttons(int *floors, int screen_height);
void draw_grid();