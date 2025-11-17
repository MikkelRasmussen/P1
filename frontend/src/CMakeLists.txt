add_executable(main main.c)
target_link_libraries(main
  PRIVATE
      raylib
      m
      glfw
      nfd::nfd
  PUBLIC 
      utils
      camera
      project
)

if(UNIX AND NOT APPLE)
    target_link_libraries(main PRIVATE m dl pthread X11 Xrandr Xi Xxf86vm Xinerama Xcursor)
endif()