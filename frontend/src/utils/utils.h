#pragma once
#include "raylib.h"

void update_mouse_drag(Camera2D *camera);
void update_camera_offset(Camera2D *camera, int *width, int *height);
int did_screen_size_change(int *width, int *height);
void draw_centered_text(char *text, int x, int y, int font_size);