add_library(camera camera.c)
target_link_libraries(
  camera
  PRIVATE
  raylib
  m
  glfw
)