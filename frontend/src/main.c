#include "raylib.h"
#include "utils/utils.h"

int main(void) {
  int screen_width = 1280;
  int screen_height = 720;
  Camera2D camera = {0};
  camera.offset = (Vector2){screen_width / 2.0f, screen_height / 2.0f};
  camera.rotation = 0.0f;
  camera.zoom = 1.0f;

  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(screen_width, screen_height, "Parking Manager");

  SetTargetFPS(60);

  // Main game loop
  while (!WindowShouldClose()) {
    // Update
    update_camera_zoom(&camera);
    update_camera_offset(&camera, &screen_width, &screen_height);
    update_mouse_drag(&camera);

    // Draw
    BeginDrawing();
    ClearBackground(RAYWHITE);

    BeginMode2D(camera);
    draw_centered_text("Parking Manager Frontend Window!", 0, 0, 20);
    EndMode2D();

    EndDrawing();
  }

  // De-Initialization
  CloseWindow(); // Close window and OpenGL context

  return 0;
}