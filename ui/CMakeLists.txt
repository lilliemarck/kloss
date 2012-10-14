find_package(Qt4 REQUIRED QtCore QtGui QtOpenGl)

include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

# Remove warnings from Qt headers by treating them as system headers
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -isystem ${QT_INCLUDE_DIR}")

qt4_wrap_cpp(ui_MOC_SOURCES ui_qt.h)

add_library(ui application.cpp ui.cpp ${ui_MOC_SOURCES})
target_link_libraries(ui ${QT_LIBRARIES})
