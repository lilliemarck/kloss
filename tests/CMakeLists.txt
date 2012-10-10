enable_testing()

find_package(GTest REQUIRED)
find_package(OpenGL)

include_directories(${GTEST_INCLUDE_DIRS})

add_executable(test
    kloss/group_test.cpp)

target_link_libraries(test ${GTEST_BOTH_LIBRARIES} ${OPENGL_gl_LIBRARY} kloss)

add_test(test test)
