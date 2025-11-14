#include "camera/camera.h"
#include "definitions.h"
#include "raylib.h"
#include "utils/utils.h"
#include <nfd.h>
#include <stdlib.h>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

int main(void) {
  Project *project = NULL;
  int tool_index = 0;

  NFD_Init();
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, "Parking Manager");
  SetTargetFPS(60);

  GuiLoadStyle("../style.rgs");
  GuiLoadIcons("../icons.rgi", false);
  set_window_icon();

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
                              &render_width, &render_height);
    update_camera(&camera, 0, TOOL_BAR_HEIGHT, render_width, render_height);
   
    if (tool_index == TOOL_INSPECT && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
    Vector2 delta = GetMouseDelta();
    camera.offset.x += delta.x;
    camera.offset.y += delta.y;
}

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
    draw_floor_buttons(project);
    draw_tab_bar(&project);
    draw_tool_bar(&tool_index, render_width);
    draw_inspector(render_width);

    EndDrawing();
  }

  UnloadRenderTexture(camera_texture);

  // De-Initialization
  CloseWindow(); // Close window and OpenGL context

  free(project);

  return 0;
}