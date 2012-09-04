#pragma once

#include <kloss/keypair.h>
#include <ui/ui.h>

namespace kloss {
namespace creator {

class gl_widget;

class move_camera_tool
{
public:
    move_camera_tool(gl_widget& parent);
    virtual ~move_camera_tool();

    void key_press_event(ui_key key);
    void key_release_event(ui_key key);

private:
    static void on_timer(void* data, ui_timer* timer);

    gl_widget& parent_;
    KeyPair* backward_forward_;
    KeyPair* left_right_;
    ui_timer* timer_;
};

} // namespace creator
} // namespace kloss
