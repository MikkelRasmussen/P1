#include "camera/camera.h"
#include "raylib.h"
#include "utils/utils.h"

int main(void) {
  int screen_width = 1280;
  int screen_height = 720;

  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(screen_width, screen_height, "Parking Manager");

  set_window_icon();
  SetTargetFPS(60);

  int render_width = screen_width / 2;
  int render_height = screen_height;
  RenderTexture camera_texture = LoadRenderTexture(render_width, render_height);

  Camera2D camera = {.offset = {render_width / 2.0f, render_height / 2.0f},
                     .target = {0, 0},
                     .rotation = 0.0f,
                     .zoom = 1.0f};

  Rectangle camera_rect = {0.0f, 0.0f, (float)render_width,
                           (float)-render_height};

  // Main game loop
  while (!WindowShouldClose()) {
    // Update
    double w_pct = render_width / (float)screen_width;
    double h_pct = render_height / (float)screen_height;
    int did_screen_size_change =
        get_screen_size_change(&screen_width, &screen_height);

    if (did_screen_size_change) {
      int old_render_width = render_width;
      int old_render_height = render_height;
      render_width = screen_width * w_pct;
      render_height = screen_height * h_pct;

      UnloadRenderTexture(camera_texture);

      camera_texture = LoadRenderTexture(render_width, render_height);
      camera_rect =
          (Rectangle){0.0f, 0.0f, (float)render_width, (float)-render_height};

      update_camera_offset(&camera, old_render_width, old_render_height,
                           render_width, render_height);
    }

    update_camera_zoom(&camera);
    update_mouse_drag(&camera);

    // Render Camera to Texture
    BeginTextureMode(camera_texture);
    ClearBackground(RAYWHITE);
    BeginMode2D(camera);
    DrawLineEx((Vector2){-render_width, 0}, (Vector2){render_width, 0}, 2,
               GRAY);
    DrawLineEx((Vector2){0, -render_height}, (Vector2){0, render_height}, 2,
               GRAY);
    draw_centered_text("Parking Manager Frontend Window!", 0, 0, 20);
    EndMode2D();
    EndTextureMode();

    // Draw
    BeginDrawing();
    ClearBackground(WHITE);
    DrawTextureRec(camera_texture.texture, camera_rect, (Vector2){0, 0}, WHITE);
    EndDrawing();
  }

  UnloadRenderTexture(camera_texture);

  // De-Initialization
  CloseWindow(); // Close window and OpenGL context

  return 0;
}