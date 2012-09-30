find_package(OpenGL)

add_executable(test
    kloss/group_suite.c
    test.c)

target_link_libraries(test check ${OPENGL_gl_LIBRARY} kloss)
