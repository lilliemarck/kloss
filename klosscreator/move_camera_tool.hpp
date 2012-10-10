#pragma once

#include <kloss/key_pair.hpp>
#include <klosscreator/tool.hpp>

class main_window;
class ui_timer;

class move_camera_tool : public tool
{
public:
    move_camera_tool(main_window& window);
    ~move_camera_tool() override;

    void key_pressed(ui_key key) override;
    void key_released(ui_key key) override;

private:
    static void on_timer(void* data, ui_timer* timer);

    main_window& window_;
    key_pair backward_forward_;
    key_pair left_right_;
    ui_timer* timer_;
};
