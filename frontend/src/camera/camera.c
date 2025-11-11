#include "../utils/utils.h"
#include "raylib.h"
#include "raymath.h"

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
