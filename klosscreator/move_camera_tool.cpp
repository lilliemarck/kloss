#include "move_camera_tool.hpp"
#include <kloss/camera.h>
#include <klosscreator/gl_widget.hpp>

namespace kloss {
namespace creator {
namespace {

float const delta_time = 1.0f / 10.0f;

void update_key_pair(KeyPair* key_pair, bool pressed, ui_key event, ui_key first, ui_key second)
{
    if (event == first)
    {
        KeyPairSetFirst(key_pair, pressed);
    }
    else if (event == second)
    {
        KeyPairSetSecond(key_pair, pressed);
    }
}

} // namespace

move_camera_tool::move_camera_tool(gl_widget& parent)
    : parent_(parent)
    , backward_forward_(CreateKeyPair())
    , left_right_(CreateKeyPair())
    , timer_(nullptr)
{
}

move_camera_tool::~move_camera_tool()
{
    ui_stop_timer(timer_);
    DestroyKeyPair(backward_forward_);
    DestroyKeyPair(left_right_);
}

void move_camera_tool::key_press_event(ui_key key)
{
    update_key_pair(backward_forward_, true, key, UI_KEY_S, UI_KEY_W);
    update_key_pair(left_right_,       true, key, UI_KEY_A, UI_KEY_D);

    if (KeyPairValue(backward_forward_) != 0.0f || KeyPairValue(left_right_) != 0.0f)
    {
        if (!timer_)
        {
            timer_ = ui_start_timer(1.0f / delta_time, &move_camera_tool::on_timer, this);
        }
    }
}

void move_camera_tool::key_release_event(ui_key key)
{
    update_key_pair(backward_forward_, false, key, UI_KEY_S, UI_KEY_W);
    update_key_pair(left_right_,       false, key, UI_KEY_A, UI_KEY_D);

    if (KeyPairValue(backward_forward_) == 0.0f && KeyPairValue(left_right_) == 0.0f)
    {
        ui_stop_timer(timer_);
        timer_ = nullptr;
    }
}

void move_camera_tool::on_timer(void* data, ui_timer* timer)
{
    move_camera_tool *self = static_cast<move_camera_tool*>(data);

    if (timer == self->timer_)
    {
        Camera* camera = self->parent_.camera();
        MoveCameraForward(camera, KeyPairValue(self->backward_forward_) * delta_time);
        MoveCameraSideways(camera, KeyPairValue(self->left_right_) * delta_time);
        ui_update_widget(self->parent_.glwidget());
    }
}

} // namespace kloss
} // namespace creator
