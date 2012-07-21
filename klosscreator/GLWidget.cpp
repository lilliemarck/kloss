#include "GLWidget.hpp"

#include <QMouseEvent>

namespace kloss {
namespace creator {

GLWidget::GLWidget(QWidget* parent) : QGLWidget(parent)
{
    camera_.position.set(0.0f, 0.0f, 4.0f);
}

void GLWidget::initializeGL()
{
}

void GLWidget::mousePressEvent(QMouseEvent* event)
{
    mouseOrigin_ = event->posF();
}

void GLWidget::mouseMoveEvent(QMouseEvent* event)
{
    QPointF move = event->posF() - mouseOrigin_;

    float widgetSize = minorSize(*this);
    float degreesPerPixel = 1.0f / widgetSize;
    camera_.yaw -= move.x() * degreesPerPixel;
    camera_.pitch -= move.y() * degreesPerPixel;

    mouseOrigin_ = event->posF();
    update();
}

void GLWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0f, float(width) / float(height), 0.1f, 1000.0f);
}

void GLWidget::paintGL()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(inverse(world_transform(camera_)).data());

    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_POLYGON);
    glVertex2i(0, 0);
    glVertex2i(1, 0);
    glVertex2i(1, 1);
    glVertex2i(0, 1);
    glEnd();
}

float minorSize(QWidget const& widget)
{
    return std::min(widget.width(), widget.height());
}

} // namespace kloss
} // namespace creator
