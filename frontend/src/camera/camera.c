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
  if (get_is_outside_renderer(render_x, render_y, render_width, render_height))
    return;

  update_camera_zoom(camera, render_x, render_y);
  update_camera_drag(camera);
}

int get_is_outside_renderer(int render_x, int render_y, int render_width,
                            int render_height) {
  Vector2 mouseScreenPos = GetMousePosition();
  Vector2 mouseRenderPos =
      (Vector2){mouseScreenPos.x - render_x, mouseScreenPos.y - render_y};

  return mouseRenderPos.x < 0 || mouseRenderPos.x > render_width ||
         mouseRenderPos.y < 0 || mouseRenderPos.y > render_height;
}

void update_camera_offset(Camera2D *camera, int old_width, int old_height,
                          int width, int height) {
  Vector2 offset = (Vector2){camera->offset.x - old_width * .5,
                             camera->offset.y - old_height * .5};

  camera->offset = (Vector2){width * .5 + offset.x, height * .5 + offset.y};
}

void update_camera_zoom(Camera2D *camera, int render_x, int render_y) {
  int wheel = GetMouseWheelMove();
  if (wheel == 0)
    return;

  Vector2 mouse_screen_pos = GetMousePosition();
  Vector2 mouse_render_pos =
      (Vector2){mouse_screen_pos.x - render_x, mouse_screen_pos.y - render_y};

  Vector2 mouse_world_pos = GetScreenToWorld2D(mouse_render_pos, *camera);
  camera->offset = mouse_render_pos;
  camera->target = mouse_world_pos;

  float scale = 0.2f * wheel;
  float zoom_factor = expf(logf(camera->zoom) + scale);
  camera->zoom = Clamp(zoom_factor, 0.25f, 8.0f);
}

void update_camera_drag(Camera2D *camera) {
  // Only middle mouse drags the camera
  if (!IsMouseButtonDown(MOUSE_BUTTON_MIDDLE))
    return;

  Vector2 delta = GetMouseDelta();
  camera->offset.x += delta.x;
  camera->offset.y += delta.y;
}
