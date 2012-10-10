#include "rotate_camera_tool.hpp"
#include <kloss/camera.hpp>
#include <klosscreator/main_window.hpp>
#include <algorithm>

rotate_camera_tool::rotate_camera_tool(main_window& window)
    : window_(window)
    , is_dragging_(false)
{
}

void rotate_camera_tool::mouse_pressed(ui_mouseevent const& event)
{
    if (event.button == UI_MOUSEBUTTON_RIGHT)
    {
        mouse_origin_x_ = event.x;
        mouse_origin_y_ = event.y;
        is_dragging_ = true;
    }
}

void rotate_camera_tool::mouse_released(ui_mouseevent const& event)
{
    if (event.button == UI_MOUSEBUTTON_RIGHT)
    {
        is_dragging_ = false;
    }
}

static float minor_size(ui_glwidget const* widget)
{
    return std::min(ui_widget_width(widget), ui_widget_height(widget));
}

void rotate_camera_tool::mouse_moved(ui_mouseevent const& event)
{
    if (is_dragging_)
    {
        camera& camera      = window_.get_camera();
        float   rotx        = event.x - mouse_origin_x_;
        float   roty        = event.y - mouse_origin_y_;
        float   degperpixel = 1.0f / minor_size(window_.get_gl_widget());

        rotate_yaw   (camera, -rotx * degperpixel);
        rotate_pitch (camera, -roty * degperpixel);

        mouse_origin_x_ = event.x;
        mouse_origin_y_ = event.y;

        ui_update_widget(window_.get_gl_widget());
    }
}
