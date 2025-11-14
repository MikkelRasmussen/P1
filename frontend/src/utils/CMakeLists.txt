add_library(utils STATIC utils.c utils.h)
target_link_libraries(utils
  PRIVATE
  raylib
)