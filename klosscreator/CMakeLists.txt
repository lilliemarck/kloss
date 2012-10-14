find_package(OpenGL)
find_package(Qt4 REQUIRED QtCore QtGui)

include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -isystem ${QT_INCLUDE_DIR}")

add_library(klosscreator
    block_selection.cpp
    constrain.cpp
    corner_selection.cpp
    document.cpp
    group_algorithm.cpp
    main_window.cpp
    move_block_tool.cpp
    move_camera_tool.cpp
    move_vertex_tool.cpp
    new_block_tool.cpp
    pick.cpp
    rotate_camera_tool.cpp
    vertex_array.cpp)

target_link_libraries(klosscreator GL kloss ui ${OPENGL_glu_LIBRARY})

add_executable(klosscreator-exe main.cpp)
target_link_libraries(klosscreator-exe klosscreator)
set_target_properties(klosscreator-exe PROPERTIES OUTPUT_NAME klosscreator)
