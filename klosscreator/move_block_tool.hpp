#pragma once

#include <cml/cml.h>
#include <kloss/block.hpp>
#include <kloss/world.hpp>
#include <klosscreator/block_selection.hpp>
#include <klosscreator/tool.hpp>

namespace kloss {

struct pick;

namespace creator {

class move_block_tool : public tool
{
public:
    move_block_tool(gl_widget& parent);

    void mouse_press_event(QMouseEvent const& event) override;
    void mouse_release_event(QMouseEvent const& event) override;
    void mouse_move_event(QMouseEvent const& event) override;
    void paint_gl() override;

private:
    gl_widget& parent_;
    block_selection selection_;
    boost::optional<cml::vector3f> reference_;
    boost::optional<block_selection::backup_type> drag_;
};

} // namespace creator
} // namespace kloss
