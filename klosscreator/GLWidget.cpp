#include "GLWidget.hpp"
#include <QMouseEvent>
#include <kloss/geometry.hpp>

namespace kloss {
namespace creator {
namespace {

float const deltaTime = 1.0f / 10.0f;

void updateKeyPair(key_pair& key_pair, bool pressed, QKeyEvent const* event, Qt::Key first, Qt::Key second)
{
    if (event->key() == first)
    {
        key_pair.set_first(pressed);
    }
    else if (event->key() == second)
    {
        key_pair.set_second(pressed);
    }
}

} // namespace

GLWidget::GLWidget(QWidget* parent)
    : QGLWidget(parent)
    , grid_(make_grid(10))
    , cursor_(make_cursor(0.125f))
{
    camera_.set_position({0.0f, 0.0f, 4.0f});
}

void GLWidget::initializeGL()
{
}

void GLWidget::keyPressEvent(QKeyEvent* event)
{
    updateKeyPair(backwardForward_, true, event, Qt::Key_S, Qt::Key_W);
    updateKeyPair(leftRight_,       true, event, Qt::Key_A, Qt::Key_D);

    if (backwardForward_.value() != 0.0f || leftRight_.value() != 0.0f)
    {
        timer_.start(1.0f / deltaTime, this);
    }
}

void GLWidget::keyReleaseEvent(QKeyEvent* event)
{
    updateKeyPair(backwardForward_, false, event, Qt::Key_S, Qt::Key_W);
    updateKeyPair(leftRight_,       false, event, Qt::Key_A, Qt::Key_D);

    if (backwardForward_.value() == 0.0f && leftRight_.value() == 0.0f)
    {
        timer_.stop();
    }
}

void GLWidget::timerEvent(QTimerEvent* event)
{
    QGLWidget::timerEvent(event);

    if (event->timerId() == timer_.timerId())
    {
        move_forward(camera_, backwardForward_.value() * deltaTime);
        move_sideways(camera_, leftRight_.value() * deltaTime);
        update();
    }
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
    rotate_yaw(camera_, -move.x() * degreesPerPixel);
    rotate_pitch(camera_, -move.y() * degreesPerPixel);

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

    draw(grid_);
    drawCursor();
}

void GLWidget::drawCursor() const
{
    if (auto position = intersect_ground_plane(to_ray(camera_)))
    {
        draw(cursor_, *position);
    }
}

float minorSize(QWidget const& widget)
{
    return std::min(widget.width(), widget.height());
}

} // namespace kloss
} // namespace creator
