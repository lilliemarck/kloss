#pragma once

#include <boost/optional.hpp>
#include <kloss/math.h>
#include <klosscreator/tool.hpp>

namespace kloss {
namespace creator {

class new_block_tool : public tool
{
public:
    new_block_tool(gl_widget& parent);

    void mouse_press_event(ui_mouseevent const *event) override;
    void mouse_move_event(ui_mouseevent const *event) override;
    void paint_gl() override;

private:
    gl_widget& parent_;
    boost::optional<Vec3> cursor_position_;
};

} // namespace creator
} // namespace kloss
