#pragma once

#include <boost/optional.hpp>
#include <cml/cml.h>
#include <klosscreator/tool.hpp>

namespace kloss {
namespace creator {

class new_block_tool : public tool
{
public:
    new_block_tool(gl_widget& parent);

    void mouse_press_event(QMouseEvent const& event) override;
    void mouse_move_event(QMouseEvent const& event) override;
    void paint_gl() override;

private:
    gl_widget& parent_;
    boost::optional<cml::vector3f> cursor_position_;
};

} // namespace creator
} // namespace kloss
