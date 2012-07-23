#include "gl_widget.hpp"
#include <QMouseEvent>
#include <kloss/block.hpp>
#include <kloss/geometry.hpp>

namespace kloss {
namespace creator {
namespace {

float const delta_time = 1.0f / 10.0f;

void update_key_pair(key_pair& key_pair, bool pressed, QKeyEvent const* event, Qt::Key first, Qt::Key second)
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

gl_widget::gl_widget(QWidget* parent)
    : QGLWidget(parent)
    , grid_(make_grid(10))
    , cursor_(make_cursor(0.125f))
{
    camera_.set_position({0.0f, -4.0f, 2.0f});
}

void gl_widget::use_new_block_tool()
{
}

void gl_widget::use_move_block_tool()
{
}

void gl_widget::initializeGL()
{
    glEnable(GL_CULL_FACE);
}

void gl_widget::keyPressEvent(QKeyEvent* event)
{
    update_key_pair(backward_forward_, true, event, Qt::Key_S, Qt::Key_W);
    update_key_pair(left_right_,       true, event, Qt::Key_A, Qt::Key_D);

    if (backward_forward_.value() != 0.0f || left_right_.value() != 0.0f)
    {
        timer_.start(1.0f / delta_time, this);
    }
}

void gl_widget::keyReleaseEvent(QKeyEvent* event)
{
    update_key_pair(backward_forward_, false, event, Qt::Key_S, Qt::Key_W);
    update_key_pair(left_right_,       false, event, Qt::Key_A, Qt::Key_D);

    if (backward_forward_.value() == 0.0f && left_right_.value() == 0.0f)
    {
        timer_.stop();
    }
}

void gl_widget::timerEvent(QTimerEvent* event)
{
    QGLWidget::timerEvent(event);

    if (event->timerId() == timer_.timerId())
    {
        move_forward(camera_, backward_forward_.value() * delta_time);
        move_sideways(camera_, left_right_.value() * delta_time);
        update();
    }
}

void gl_widget::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        if (auto position = cursor_position())
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
        mouse_origin_ = event->posF();
    }
}

void gl_widget::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::RightButton)
    {
        mouse_origin_.reset();
    }
}

void gl_widget::mouseMoveEvent(QMouseEvent* event)
{
    if (mouse_origin_)
    {
        QPointF move = event->posF() - *mouse_origin_;

        float widget_size = minor_size(*this);
        float degrees_per_pixel = 1.0f / widget_size;
        rotate_yaw(camera_, -move.x() * degrees_per_pixel);
        rotate_pitch(camera_, -move.y() * degrees_per_pixel);

        mouse_origin_ = event->posF();
        update();
    }
}

void gl_widget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0f, float(width) / float(height), 0.1f, 1000.0f);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
}

void gl_widget::paintGL()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(inverse(world_transform(camera_)).data());

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    draw(grid_);
    world_.draw();
    draw_cursor();
}

boost::optional<cml::vector3f> gl_widget::cursor_position() const
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

void gl_widget::draw_cursor() const
{
    if (auto position = cursor_position())
    {
        draw(cursor_, *position);
    }
}

float minor_size(QWidget const& widget)
{
    return std::min(widget.width(), widget.height());
}

} // namespace kloss
} // namespace creator
