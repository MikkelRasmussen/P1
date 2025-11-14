add_executable(main main.c)
target_link_libraries(
  main
  PRIVATE
  raylib
  nfd::nfd 
  PUBLIC 
  utils 
  camera 
  project
)