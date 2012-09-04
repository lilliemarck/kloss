find_package(OpenGL)

add_executable(klosscreator
    blockselection.c
    constrain.c
    cornerselection.c
    document.c
    document_lock.cpp
    gl_widget.cpp
    main.cpp
    main_window.cpp
    move_block_tool.cpp
    move_camera_tool.cpp
    move_vertex_tool.cpp
    new_block_tool.cpp
    pick.c
    turn_camera_tool.cpp
    vertex_array.cpp)

target_link_libraries(klosscreator GL kloss ui ${OPENGL_glu_LIBRARY})
