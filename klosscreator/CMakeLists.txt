find_package(OpenGL)

add_library(klosscreator
    blockselection.c
    constrain.c
    cornerselection.c
    document.c
    mainwindow.c
    moveblock.c
    movecamera.c
    movevertex.c
    newblock.c
    pick.c
    rotatecamera.c
    vertexarray.c)

target_link_libraries(klosscreator GL kloss ui ${OPENGL_glu_LIBRARY})

add_executable(klosscreator-exe main.c)
target_link_libraries(klosscreator-exe klosscreator)
set_target_properties(klosscreator-exe PROPERTIES OUTPUT_NAME klosscreator)
