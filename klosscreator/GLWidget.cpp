#include "GLWidget.hpp"

namespace kloss {
namespace creator {

GLWidget::GLWidget(QWidget* parent) : QGLWidget(parent)
{
}

void GLWidget::initializeGL()
{
}

void GLWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, width, 0, height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_POLYGON);
    glVertex2i(  0,   0);
    glVertex2i(100,   0);
    glVertex2i(100, 100);
    glVertex2i(  0, 100);
    glEnd();
}

} // namespace kloss
} // namespace creator
