#include "move_camera_tool.hpp"
#include <QKeyEvent>
#include <klosscreator/gl_widget.hpp>

namespace kloss {
namespace creator {
namespace {

float const delta_time = 1.0f / 10.0f;

void update_key_pair(key_pair& key_pair, bool pressed, QKeyEvent const& event, Qt::Key first, Qt::Key second)
{
    if (event.key() == first)
    {
        key_pair.set_first(pressed);
    }
    else if (event.key() == second)
    {
        key_pair.set_second(pressed);
    }
}

} // namespace

move_camera_tool::move_camera_tool(gl_widget& parent) : parent_(parent)
{
}

void move_camera_tool::key_press_event(QKeyEvent const& event)
{
    update_key_pair(backward_forward_, true, event, Qt::Key_S, Qt::Key_W);
    update_key_pair(left_right_,       true, event, Qt::Key_A, Qt::Key_D);

    if (backward_forward_.value() != 0.0f || left_right_.value() != 0.0f)
    {
        timer_.start(1.0f / delta_time, this);
    }
}

void move_camera_tool::key_release_event(QKeyEvent const& event)
{
    update_key_pair(backward_forward_, false, event, Qt::Key_S, Qt::Key_W);
    update_key_pair(left_right_,       false, event, Qt::Key_A, Qt::Key_D);

    if (backward_forward_.value() == 0.0f && left_right_.value() == 0.0f)
    {
        timer_.stop();
    }
}

void move_camera_tool::timerEvent(QTimerEvent* event)
{
    QObject::timerEvent(event);

    if (event->timerId() == timer_.timerId())
    {
        camera& camera = parent_.camera();
        move_forward(camera, backward_forward_.value() * delta_time);
        move_sideways(camera, left_right_.value() * delta_time);
        parent_.update();
    }
}

} // namespace kloss
} // namespace creator
