#include "raylib.h"

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

void update_mouse_drag(Camera2D *camera) {
  int is_middle_mouse_down = IsMouseButtonDown(MOUSE_BUTTON_MIDDLE);
  if (!is_middle_mouse_down)
    return;

  Vector2 delta = GetMouseDelta();
  Vector2 offset = camera->offset;
  camera->offset = (Vector2){offset.x + delta.x, offset.y + delta.y};
}

void draw_centered_text(char *text, int x, int y, int font_size) {
  Vector2 text_size = MeasureTextEx(GetFontDefault(), text, font_size, 0);
  DrawText(text, -text_size.x * .5, -text_size.y * .5, font_size, LIGHTGRAY);
}

void set_window_icon() {
  Image icon = LoadImage("../icon.png");
  SetWindowIcon(icon);
  UnloadImage(icon);
}