#include "camera.h"
#include "raymath.h"

Camera2D init_camera(int render_width, int render_height) {
  return (Camera2D){.offset = {render_width / 2.0f, render_height / 2.0f},
                    .target = {0, 0},
                    .rotation = 0.0f,
                    .zoom = 1.0f};
}

void update_camera(Camera2D *camera, int render_x, int render_y,
                   int render_width, int render_height) {
  update_camera_zoom(camera, render_x, render_y, render_width, render_height);
  update_camera_drag(camera);
}

void update_camera_offset(Camera2D *camera, int old_width, int old_height,
                          int width, int height) {
  Vector2 offset = (Vector2){camera->offset.x - old_width * .5,
                             camera->offset.y - old_height * .5};

  camera->offset = (Vector2){width * .5 + offset.x, height * .5 + offset.y};
}

void update_camera_zoom(Camera2D *camera, int render_x, int render_y,
                        int render_width, int render_height) {
  int wheel = GetMouseWheelMove();
  if (wheel == 0)
    return;

  Vector2 mouseScreenPos = GetMousePosition();
  Vector2 mouseRenderPos =
      (Vector2){mouseScreenPos.x - render_x, mouseScreenPos.y - render_y};

  int is_outside_renderer =
      mouseRenderPos.x < 0 || mouseRenderPos.x > render_width ||
      mouseRenderPos.y < 0 || mouseRenderPos.y > render_height;
  if (is_outside_renderer)
    return;

  Vector2 mouseWorldPos = GetScreenToWorld2D(mouseRenderPos, *camera);
  camera->offset = mouseRenderPos;
  camera->target = mouseWorldPos;

  float scale = 0.2f * wheel;
  camera->zoom = Clamp(expf(logf(camera->zoom) + scale), 0.125f, 64.0f);
}

void update_camera_drag(Camera2D *camera) {
  int is_middle_mouse_down = IsMouseButtonDown(MOUSE_BUTTON_MIDDLE);
  if (!is_middle_mouse_down)
    return;

  Vector2 delta = GetMouseDelta();
  Vector2 offset = camera->offset;
  camera->offset = (Vector2){offset.x + delta.x, offset.y + delta.y};
}