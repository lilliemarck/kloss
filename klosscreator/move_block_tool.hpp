#pragma once

#include <boost/optional.hpp>
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
    void paint_gl() override;

private:
    gl_widget& parent_;
    boost::optional<block> block_;
};

} // namespace creator
} // namespace kloss
