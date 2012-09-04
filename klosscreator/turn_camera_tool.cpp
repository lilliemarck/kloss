#include "turn_camera_tool.hpp"
#include <kloss/camera.h>
#include <klosscreator/gl_widget.hpp>

namespace kloss {
namespace creator {

static vec2i subtract(vec2i const& lhs, vec2i const& rhs)
{
    return {lhs.x - rhs.x, lhs.y - rhs.y};
}

turn_camera_tool::turn_camera_tool(gl_widget& parent) : parent_(parent)
{
}

void turn_camera_tool::mouse_press_event(ui_mouseevent const *event)
{
    if (event->button == UI_MOUSEBUTTON_RIGHT)
    {
        mouse_origin_ = vec2i{event->x, event->y};
    }
}

void turn_camera_tool::mouse_release_event(ui_mouseevent const *event)
{
    if (event->button == UI_MOUSEBUTTON_RIGHT)
    {
        mouse_origin_.reset();
    }
}

static float minor_size(ui_glwidget const* widget)
{
    return std::min(ui_widget_width(widget), ui_widget_height(widget));
}

void turn_camera_tool::mouse_move_event(ui_mouseevent const *event)
{
    if (mouse_origin_)
    {
        vec2i mousepos = {event->x, event->y};
        vec2i turn = subtract(mousepos, *mouse_origin_);
        float degrees_per_pixel = 1.0f / minor_size(parent_.glwidget());

        Camera* camera = parent_.camera();
        RotateCameraYaw(camera, -turn.x * degrees_per_pixel);
        RotateCameraPitch(camera, -turn.y * degrees_per_pixel);

        mouse_origin_ = mousepos;
        ui_update_widget(parent_.glwidget());
    }
}

} // namespace kloss
} // namespace creator
