#include "move_camera_tool.hpp"
#include <kloss/camera.hpp>
#include <klosscreator/main_window.hpp>
#include <cstdlib>

float const deltatime = 1.0f / 10.0f;

static void update_key_pair(key_pair& pair, bool pressed, ui_key event, ui_key first, ui_key second)
{
    if (event == first)
    {
        pair.set_first(pressed);
    }
    else if (event == second)
    {
        pair.set_second(pressed);
    }
}

move_camera_tool::move_camera_tool(main_window& window)
    : window_(window)
    , timer_(nullptr)
{
}

move_camera_tool::~move_camera_tool()
{
    ui_stop_timer(timer_);
}

void move_camera_tool::key_pressed(ui_key key)
{
    update_key_pair(backward_forward_, true, key, UI_KEY_S, UI_KEY_W);
    update_key_pair(left_right_,       true, key, UI_KEY_A, UI_KEY_D);

    if (backward_forward_.value() != 0.0f || left_right_.value() != 0.0f)
    {
        if (!timer_)
        {
            timer_ = ui_start_timer(1.0f / deltatime, on_timer, this);
        }
    }
}

void move_camera_tool::key_released(ui_key key)
{
    update_key_pair(backward_forward_, false, key, UI_KEY_S, UI_KEY_W);
    update_key_pair(left_right_,       false, key, UI_KEY_A, UI_KEY_D);

    if (backward_forward_.value() == 0.0f && left_right_.value() == 0.0f)
    {
        ui_stop_timer(timer_);
        timer_ = nullptr;
    }
}

void move_camera_tool::on_timer(void* data, ui_timer* timer)
{
    move_camera_tool* self = static_cast<move_camera_tool*>(data);

    if (timer == self->timer_)
    {
        camera& camera = self->window_.get_camera();
        move_forward(camera, self->backward_forward_.value() * deltatime);
        move_sideways(camera, self->left_right_.value() * deltatime);
        ui_update_widget(self->window_.get_gl_widget());
    }
}
