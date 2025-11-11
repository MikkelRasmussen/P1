#include "raylib.h"
#include "raymath.h"

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

void update_camera_offset(Camera2D *camera, int *width, int *height) {
  Vector2 offset = (Vector2){camera->offset.x - *width * .5,
                             camera->offset.y - *height * .5};

  int did_screen_size_change = get_screen_size_change(width, height);
  if (!did_screen_size_change)
    return;

  camera->offset = (Vector2){*width * .5 + offset.x, *height * .5 + offset.y};
}

void update_camera_zoom(Camera2D *camera) {
  int wheel = GetMouseWheelMove();
  if (wheel == 0)
    return;

  Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), *camera);
  camera->offset = GetMousePosition();
  camera->target = mouseWorldPos;

  float scale = 0.2f * wheel;
  camera->zoom = Clamp(expf(logf(camera->zoom) + scale), 0.125f, 64.0f);
}

void update_mouse_drag(Camera2D *camera) {
  int is_middle_mouse_down = IsMouseButtonDown(MOUSE_BUTTON_MIDDLE);
  if (!is_middle_mouse_down)
    return;

  Vector2 delta = GetMouseDelta();
  // delta.x /= camera->zoom;
  // delta.y /= camera->zoom;
  Vector2 offset = camera->offset;
  camera->offset = (Vector2){offset.x + delta.x, offset.y + delta.y};
}

void draw_centered_text(char *text, int x, int y, int font_size) {
  Vector2 text_size = MeasureTextEx(GetFontDefault(), text, font_size, 0);
  DrawText(text, -text_size.x * .5, -text_size.y * .5, font_size, LIGHTGRAY);
}