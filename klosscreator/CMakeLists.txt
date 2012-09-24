find_package(OpenGL)

add_executable(klosscreator
    blockselection.c
    constrain.c
    cornerselection.c
    document.c
    main.c
    mainwindow.c
    moveblock.c
    movecamera.c
    movevertex.c
    newblock.c
    pick.c
    rotatecamera.c
    vertexarray.c)

target_link_libraries(klosscreator GL kloss ui ${OPENGL_glu_LIBRARY})
