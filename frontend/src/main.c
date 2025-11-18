#include "camera/camera.h"
#include "definitions.h"
#include "parking-spot/parking-spot.h"
#include "raylib.h"
#include "utils/utils.h"
#include <math.h>
#include <nfd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

int main(void) {
  Project *project = NULL;
  int tool_index = 0;

  NFD_Init();
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, "Parking Manager");
  SetTargetFPS(60);

  // char zones[] = {'A', 'B', 'C', 'D'};
  // set_project_zones(project, zones, sizeof(zones) / sizeof(zones[0]));

  GuiLoadStyle("../style.rgs");
  GuiLoadIcons("../icons.rgi", false);
  set_window_icon();

  int render_width = DEFAULT_SCREEN_WIDTH - INSPECTOR_WIDTH;
  int render_height = DEFAULT_SCREEN_HEIGHT - TAB_BAR_HEIGHT;
  RenderTexture camera_texture = LoadRenderTexture(render_width, render_height);
  Camera2D camera = init_camera(render_width, render_height);
  Rectangle camera_rect = {0.0f, 0.0f, (float)render_width,
                           (float)-render_height};

  while (!WindowShouldClose()) {
    // Update camera and screen size
    update_screen_size_change(&camera, &camera_texture, &camera_rect,
                              &render_width, &render_height);
    update_camera(&camera, 0, TAB_BAR_HEIGHT, render_width, render_height);

    // Handle inspect tool (drag camera)
    handle_inspect_tool(&camera, render_width, render_height, tool_index);

    // Handle parking tool (add parking spot on mouse click)
    handle_parking_tool(project, &camera, tool_index, 0, TAB_BAR_HEIGHT,
                        render_width, render_height);

    handle_save(project);

    // --- Drawing to render texture ---
    BeginTextureMode(camera_texture);
    ClearBackground(get_bg_color());
    BeginMode2D(camera);

    draw_grid();                 // draw background grid
    draw_parking_spots(project); // draw all parking spots

    EndMode2D();
    EndTextureMode();

    // --- Draw GUI ---
    BeginDrawing();
    ClearBackground(get_bg_color());

    // Draw Camera Texture
    DrawTextureRec(camera_texture.texture, camera_rect,
                   (Vector2){0, TAB_BAR_HEIGHT}, WHITE);
    draw_floor_buttons(project);
    draw_tab_bar(&project);
    draw_tool_bar(&tool_index, render_width);
    draw_inspector(render_width);

    EndDrawing();
  }

  // --- Clean up ---
  UnloadRenderTexture(camera_texture);
  free_project(&project);
  CloseWindow();

  return 0;
}
