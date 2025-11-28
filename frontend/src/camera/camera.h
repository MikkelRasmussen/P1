#pragma once
#include "raylib.h"

Camera2D init_camera(int render_width, int render_height);

/**
 * Updates the camera drag and zoom logic
 */
void update_camera(Camera2D *camera, Rectangle render_rect);

int get_is_outside_renderer(Rectangle render_rect);

/**
 * Updates the camera offset when resizing the window
 */
void update_camera_offset(Camera2D *camera, int old_render_width,
                          int old_render_height, int render_width,
                          int render_height);

/**
 * Updates the camera zoom when scrolling on mouse wheel
 */
void update_camera_zoom(Camera2D *camera, int render_x, int render_y);

void update_camera_drag(Camera2D *camera);
