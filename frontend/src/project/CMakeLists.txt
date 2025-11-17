add_library(project project.c)
target_link_libraries(project
  PRIVATE
  raylib
  nfd::nfd
  m
  glfw
  PUBLIC
  parking-spot
)