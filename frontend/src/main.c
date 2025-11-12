#include "camera/camera.h"
#include "definitions.h"
#include "raylib.h"
#include "utils/utils.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

int main(void) {
  int screen_width = DEFAULT_SCREEN_WIDTH;
  int screen_height = DEFAULT_SCREEN_HEIGHT;

  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(screen_width, screen_height, "Parking Manager");

  GuiLoadStyle("../style.rgs");
  set_window_icon();
  SetTargetFPS(60);

  int render_width = DEFAULT_SCREEN_WIDTH - INSPECTOR_WIDTH;
  int render_height = DEFAULT_SCREEN_HEIGHT - TOOL_BAR_HEIGHT;
  RenderTexture camera_texture = LoadRenderTexture(render_width, render_height);
  Camera2D camera = init_camera(render_width, render_height);
  Rectangle camera_rect = {0.0f, 0.0f, (float)render_width,
                           (float)-render_height};

  // Main game loop
  while (!WindowShouldClose()) {
    // Update
    update_screen_size_change(&camera, &camera_texture, &camera_rect,
                              &screen_width, &screen_height, &render_width,
                              &render_height);
    update_camera(&camera, 0, TOOL_BAR_HEIGHT, render_width, render_height);

    // Draw to Camera Render Texture
    BeginTextureMode(camera_texture);
    ClearBackground(get_bg_color());
    BeginMode2D(camera);

    draw_grid();
    draw_centered_text("Parking Manager Frontend Window!", 0, 0, 32);

    EndMode2D();
    EndTextureMode();

    // Draw to Screen
    BeginDrawing();
    ClearBackground(get_bg_color());

    // Draw Camera Texture
    DrawTextureRec(camera_texture.texture, camera_rect,
                   (Vector2){0, TOOL_BAR_HEIGHT}, WHITE);
    draw_tool_bar(screen_width);
    GuiPanel((Rectangle){render_width, TOOL_BAR_HEIGHT,
                         screen_width - render_width,
                         screen_height - TOOL_BAR_HEIGHT},
             "Inspector");
    draw_floor_buttons(2, screen_height);

    EndDrawing();
  }

  UnloadRenderTexture(camera_texture);

  // De-Initialization
  CloseWindow(); // Close window and OpenGL context

  return 0;
}