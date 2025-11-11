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

  Camera2D camera = {.offset = {screen_width / 2.0f, screen_height / 2.0f},
                     .target = {0, 0},
                     .rotation = 0.0f,
                     .zoom = 1.0f};
  RenderTexture camera_texture =
      LoadRenderTexture(screen_width / 2, screen_height);
  Rectangle camera_rect = {0.0f, 0.0f, (float)camera_texture.texture.width,
                           (float)-camera_texture.texture.height};

  // Main game loop
  while (!WindowShouldClose()) {
    // Update
    update_camera_zoom(&camera);
    update_camera_offset(&camera, &screen_width, &screen_height);
    update_mouse_drag(&camera);

    // Render Camera to Texture
    BeginTextureMode(camera_texture);
    ClearBackground(RAYWHITE);
    BeginMode2D(camera);
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