#include "turn_camera_tool.hpp"
#include <QMouseEvent>
#include <klosscreator/gl_widget.hpp>

namespace kloss {
namespace creator {

turn_camera_tool::turn_camera_tool(gl_widget& parent) : parent_(parent)
{
}

void turn_camera_tool::mouse_press_event(QMouseEvent const& event)
{
    if (event.button() == Qt::RightButton)
    {
        mouse_origin_ = event.pos();
    }
}

void turn_camera_tool::mouse_release_event(QMouseEvent const& event)
{
    if (event.button() == Qt::RightButton)
    {
        mouse_origin_.reset();
    }
}

void turn_camera_tool::mouse_move_event(QMouseEvent const& event)
{
    if (mouse_origin_)
    {
        QPoint turn = event.pos() - *mouse_origin_;
        float degrees_per_pixel = 1.0f / minor_size(parent_);

        camera& camera = parent_.camera();
        rotate_yaw(camera, -turn.x() * degrees_per_pixel);
        rotate_pitch(camera, -turn.y() * degrees_per_pixel);

        mouse_origin_ = event.pos();
        parent_.update();
    }
}

} // namespace kloss
} // namespace creator
