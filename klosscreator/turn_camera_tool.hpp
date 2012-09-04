#pragma once

#include <boost/optional.hpp>

struct ui_mouseevent;

namespace kloss {
namespace creator {

class gl_widget;

struct vec2i
{
    int x, y;
};

class turn_camera_tool
{
public:
    turn_camera_tool(gl_widget& parent);

    void mouse_press_event(ui_mouseevent const *event);
    void mouse_release_event(ui_mouseevent const *event);
    void mouse_move_event(ui_mouseevent const *event);

private:
    gl_widget& parent_;
    boost::optional<vec2i> mouse_origin_;
};

} // namespace creator
} // namespace kloss
