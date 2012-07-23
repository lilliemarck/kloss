#pragma once

#include <cml/cml.h>
#include <kloss/block.hpp>
#include <klosscreator/tool.hpp>

namespace kloss {
namespace creator {

class move_block_tool : public tool
{
public:
    move_block_tool(gl_widget& parent);

    void mouse_press_event(QMouseEvent* event) override;
    void mouse_release_event(QMouseEvent* event) override;
    void mouse_move_event(QMouseEvent* event) override;
    void paint_gl() override;

private:
    struct drag
    {
        cml::vector3f drag_origin;
        block original_block;
    };

    gl_widget& parent_;
    block_ptr block_;
    boost::optional<drag> drag_;
};

} // namespace creator
} // namespace kloss
