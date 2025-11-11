#pragma once
#include "raylib.h"

void update_camera_offset(Camera2D *camera, int old_width, int old_height,
                          int width, int height);
void update_camera_zoom(Camera2D *camera);