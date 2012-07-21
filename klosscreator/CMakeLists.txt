find_package(Qt4 REQUIRED QtGui QtOpenGl)
find_package(OpenGL)

include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

# Remove warnings from Qt headers by treating them as system headers
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -isystem ${QT_INCLUDE_DIR}")

add_executable(klosscreator
    GLWidget.cpp
    main.cpp)

target_link_libraries(klosscreator kloss ${QT_LIBRARIES} ${OPENGL_glu_LIBRARY})
