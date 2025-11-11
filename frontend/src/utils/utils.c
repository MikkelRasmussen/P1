#include "raylib.h"

int did_screen_size_change(int *width, int *height) {
  int new_width = GetScreenWidth();
  int new_height = GetScreenHeight();

  int did_change = *width != new_width || *height != new_height;
  if (!did_change)
    return 0;

  *width = new_width;
  *height = new_height;
  return 1;
}

void update_camera_offset(Camera2D *camera, int *width, int *height) {
  int old_screen_width = *width;
  int old_screen_height = *height;
  if (did_screen_size_change(width, height)) {
    Vector2 offset = (Vector2){camera->offset.x - old_screen_width * .5,
                               camera->offset.y - old_screen_height * .5};
    camera->offset = (Vector2){*width * .5 + offset.x, *height * .5 + offset.y};
  }
}

void update_mouse_drag(Camera2D *camera) {
  if (!IsMouseButtonDown(MOUSE_BUTTON_MIDDLE))
    return;

  Vector2 delta = GetMouseDelta();
  delta.x /= camera->zoom;
  delta.y /= camera->zoom;
  Vector2 offset = camera->offset;
  camera->offset = (Vector2){offset.x + delta.x, offset.y + delta.y};
}

void draw_centered_text(char *text, int x, int y, int font_size) {
  Vector2 text_size = MeasureTextEx(GetFontDefault(), text, font_size, 0);
  DrawText(text, -text_size.x * .5, -text_size.y * .5, font_size, LIGHTGRAY);
}