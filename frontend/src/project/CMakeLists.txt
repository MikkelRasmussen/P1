add_library(project project.c)
target_link_libraries(project
  PRIVATE
  raylib
  nfd::nfd
  PUBLIC
  parking-spot
)