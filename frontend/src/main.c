#include "camera/camera.h"
#include "definitions.h"
#include "raylib.h"
#include "utils/utils.h"
#include <string.h>
#include <math.h>
#include <nfd.h>
#include <stdlib.h>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

int main(void)
{
  Project project;
  init_project(&project);
  Project *project = NULL;
  int tool_index = 0;

  project.floors = 3;
  project.active_floor = 0;
  NFD_Init();
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, "Parking Manager");
  SetTargetFPS(60);

  char zones[] = {'A', 'B', 'C', 'D'};
  set_project_zones(&project, zones, sizeof(zones) / sizeof(zones[0]));

  int tool_index = 0;

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
  Rectangle camera_rect = {0.0f, 0.0f, (float)render_width, (float)render_height};

  while (!WindowShouldClose())
  {
    // Update camera and screen size
    update_screen_size_change(&camera, &camera_texture, &camera_rect,
                              &render_width, &render_height);
    update_camera(&camera, 0, TOOL_BAR_HEIGHT, render_width, render_height);

    // Handle inspect tool (drag camera)
    handle_inspect_tool(&camera, render_width, render_height, tool_index);
    // Draw Camera Texture
    DrawTextureRec(camera_texture.texture, camera_rect,
                   (Vector2){0, TOOL_BAR_HEIGHT}, WHITE);
    draw_floor_buttons(project);
    draw_tab_bar(&project);
    draw_tool_bar(&tool_index, render_width);
    draw_inspector(render_width);

    // Add parking spot on mouse click
    if (tool_index == TOOL_PARKING && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
      Vector2 mouse_pos = GetMousePosition();
      Vector2 world_pos = GetScreenToWorld2D(mouse_pos, camera);

      // Snap to grid
      world_pos.x = roundf(world_pos.x / GRID_SIZE) * GRID_SIZE;
      world_pos.y = roundf(world_pos.y / GRID_SIZE) * GRID_SIZE;

      // Define parking spot size
      float spot_width = GRID_SIZE;
      float spot_height = GRID_SIZE * 1.5f;

      // Assign first zone by default
      add_parking_spot(&project, world_pos, spot_width, spot_height, project.zones[0]);
    }

    // --- Drawing to render texture ---
    BeginTextureMode(camera_texture);
    ClearBackground(get_bg_color());
    BeginMode2D(camera);

    draw_grid();                  // draw background grid
    draw_parking_spots(&project); // draw all parking spots

    EndMode2D();
    EndTextureMode();

    // --- Draw GUI ---
    BeginDrawing();
    ClearBackground(get_bg_color());

    DrawTextureRec(camera_texture.texture, camera_rect, (Vector2){0, TOOL_BAR_HEIGHT}, WHITE);

    draw_floor_buttons(&project);             // draw floor selection
    draw_tab_bar();                           // draw tab bar
    draw_tool_bar(&tool_index, render_width); // draw tool selection
    draw_inspector(render_width);             // draw inspector panel

    EndDrawing();
  }

  // --- Clean up ---
  UnloadRenderTexture(camera_texture);
  free_project(&project);
  CloseWindow();

  return 0;
}
