#pragma once

#include <klosscreator/tool.hpp>

class main_window;

class rotate_camera_tool : public tool
{
public:
    rotate_camera_tool(main_window& window);

    void mouse_pressed(ui_mouseevent const& event) override;
    void mouse_released(ui_mouseevent const& event) override;
    void mouse_moved(ui_mouseevent const& event) override;

private:
    main_window& window_;
    bool is_dragging_;
    int mouse_origin_x_;
    int mouse_origin_y_;
};
