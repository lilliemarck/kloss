#include "move_camera_tool.hpp"
#include <QKeyEvent>
#include <kloss/camera.h>
#include <klosscreator/gl_widget.hpp>

namespace kloss {
namespace creator {
namespace {

float const delta_time = 1.0f / 10.0f;

void update_key_pair(KeyPair* key_pair, bool pressed, QKeyEvent const& event, Qt::Key first, Qt::Key second)
{
    if (event.key() == first)
    {
        KeyPairSetFirst(key_pair, pressed);
    }
    else if (event.key() == second)
    {
        KeyPairSetSecond(key_pair, pressed);
    }
}

} // namespace

move_camera_tool::move_camera_tool(gl_widget& parent)
    : parent_(parent)
    , backward_forward_(CreateKeyPair())
    , left_right_(CreateKeyPair())
{
}

move_camera_tool::~move_camera_tool()
{
    DestroyKeyPair(backward_forward_);
    DestroyKeyPair(left_right_);
}

void move_camera_tool::key_press_event(QKeyEvent const& event)
{
    update_key_pair(backward_forward_, true, event, Qt::Key_S, Qt::Key_W);
    update_key_pair(left_right_,       true, event, Qt::Key_A, Qt::Key_D);

    if (KeyPairValue(backward_forward_) != 0.0f || KeyPairValue(left_right_) != 0.0f)
    {
        timer_.start(1.0f / delta_time, this);
    }
}

void move_camera_tool::key_release_event(QKeyEvent const& event)
{
    update_key_pair(backward_forward_, false, event, Qt::Key_S, Qt::Key_W);
    update_key_pair(left_right_,       false, event, Qt::Key_A, Qt::Key_D);

    if (KeyPairValue(backward_forward_) == 0.0f && KeyPairValue(left_right_) == 0.0f)
    {
        timer_.stop();
    }
}

void move_camera_tool::timerEvent(QTimerEvent* event)
{
    QObject::timerEvent(event);

    if (event->timerId() == timer_.timerId())
    {
        Camera* camera = parent_.camera();
        MoveCameraForward(camera, KeyPairValue(backward_forward_) * delta_time);
        MoveCameraSideways(camera, KeyPairValue(left_right_) * delta_time);
        parent_.update();
    }
}

} // namespace kloss
} // namespace creator
