#include "GLWidget.hpp"
#include <QMouseEvent>
#include <kloss/block.hpp>
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
    camera_.set_position({0.0f, -4.0f, 2.0f});
}

void GLWidget::initializeGL()
{
    glEnable(GL_CULL_FACE);
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
    if (event->button() == Qt::LeftButton)
    {
        if (auto position = cursorPosition())
        {
            float x      = (*position)[0];
            float y      = (*position)[1];
            float top    = 1.0f;
            float bottom = 0.0f;

            block block;
            block[0] = {x,        y,        top, bottom};
            block[1] = {x + 1.0f, y,        top, bottom};
            block[2] = {x + 1.0f, y + 1.0f, top, bottom};
            block[3] = {x,        y + 1.0f, top, bottom};

            world_.insert(block);
            update();
        }
    }
    else if (event->button() == Qt::RightButton)
    {
        mouseOrigin_ = event->posF();
    }
}

void GLWidget::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::RightButton)
    {
        mouseOrigin_.reset();
    }
}

void GLWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (mouseOrigin_)
    {
        QPointF move = event->posF() - *mouseOrigin_;

        float widgetSize = minorSize(*this);
        float degreesPerPixel = 1.0f / widgetSize;
        rotate_yaw(camera_, -move.x() * degreesPerPixel);
        rotate_pitch(camera_, -move.y() * degreesPerPixel);

        mouseOrigin_ = event->posF();
        update();
    }
}

void GLWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0f, float(width) / float(height), 0.1f, 1000.0f);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
}

void GLWidget::paintGL()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(inverse(world_transform(camera_)).data());

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    draw(grid_);
    world_.draw();
    drawCursor();
}

boost::optional<cml::vector3f> GLWidget::cursorPosition() const
{
    if (auto position = intersect_xy_plane(to_ray(camera_)))
    {
        auto snapped = *position;

        snapped[0] = std::round(snapped[0]);
        snapped[1] = std::round(snapped[1]);

        return snapped;
    }
    else
    {
        return {};
    }
}


void GLWidget::drawCursor() const
{
    if (auto position = cursorPosition())
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
