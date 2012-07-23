find_package(Qt4 REQUIRED QtCore QtGui QtOpenGl)
find_package(OpenGL)

include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

# Remove warnings from Qt headers by treating them as system headers
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -isystem ${QT_INCLUDE_DIR}")

qt4_wrap_cpp(klosscreator_MOC_SOURCES
    gl_widget.hpp
    main_window.hpp)

add_executable(klosscreator ${klosscreator_MOC_SOURCES}
    gl_widget.cpp
    main.cpp
    main_window.cpp
    new_block_tool.cpp
    vertex_array.cpp)

target_link_libraries(klosscreator kloss ${QT_LIBRARIES} ${OPENGL_glu_LIBRARY})
