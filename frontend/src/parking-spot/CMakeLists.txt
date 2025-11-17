add_library(parking-spot parking-spot.c)
target_link_libraries(parking-spot
  PRIVATE
  raylib
  m
  glfw
  PUBLIC
  project
)
