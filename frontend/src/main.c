#include "camera/camera.h"
#include "definitions.h"
#include "project/project.h"
#include "raylib.h"
#include "selection/selection.h"
#include "utils/utils.h"
#include <math.h>
#include <nfd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

int main(void) {
  int tool_index = 0;

  NFD_Init();
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, "Parking Manager");
  SetTargetFPS(60);

  GuiLoadStyle("../style.rgs");
  GuiLoadIcons("../icons.rgi", false);
  set_window_icon();

  int render_width = DEFAULT_SCREEN_WIDTH - INSPECTOR_WIDTH;
  int render_height = DEFAULT_SCREEN_HEIGHT - TAB_BAR_HEIGHT - BUTTON_SIZE;
  init_camera(render_width, render_height);
  RenderTexture camera_texture = LoadRenderTexture(render_width, render_height);
  Rectangle camera_rect = (Rectangle){.x = 0,
                                      .y = 0,
                                      .width = (float)render_width,
                                      .height = (float)-render_height};

  while (!WindowShouldClose()) {
    Rectangle render_rect = (Rectangle){.x = 0,
                                        .y = TAB_BAR_HEIGHT,
                                        .width = render_width,
                                        .height = render_height};

    // Update camera and screen size
    update_screen_size_change(&camera_texture, &camera_rect, &render_width,
                              &render_height);
    update_camera(render_rect);

    update_selection(render_rect, tool_index);
    update_tool_selection(&tool_index);

    // Handle inspect tool (drag camera)
    handle_inspect_tool(tool_index, render_rect);

    // Handle parking tool (add parking spot on mouse click)
    handle_spot_tool(tool_index, render_rect);
    handle_road_tool(tool_index, render_rect);
    handle_entrance_tool(tool_index, render_rect);

    handle_save();

    // --- Drawing to render texture ---
    BeginTextureMode(camera_texture);
    ClearBackground(get_bg_color());
    BeginMode2D(camera);

    draw_grid();  // draw background grid
    draw_spots(); // draw all parking spots
    draw_roads();
    draw_entrances();

    // Draw parking preview
    draw_selection_preview(render_rect);
    draw_selection(render_rect);

    EndMode2D();
    EndTextureMode();

    // --- Draw GUI ---
    BeginDrawing();
    ClearBackground(get_bg_color());

    // Draw Camera Texture
    if (project != NULL) {
      DrawTextureRec(camera_texture.texture, camera_rect,
                     (Vector2){render_rect.x, render_rect.y}, WHITE);
      draw_floor_buttons();
    } else {
      draw_centered_text(
          "No project open!", (SCREEN_WIDTH - INSPECTOR_WIDTH) / 2,
          (SCREEN_HEIGHT - TAB_BAR_HEIGHT) / 2 + TAB_BAR_HEIGHT, 20);
    }

    draw_tool_bar(&tool_index, render_width);
    draw_tab_bar();
    draw_inspector(render_width);

    EndDrawing();
  }

  // --- Clean up ---
  UnloadRenderTexture(camera_texture);
  free_project();
  CloseWindow();

  return 0;
}
