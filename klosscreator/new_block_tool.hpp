#pragma once

#include <klosscreator/tool.hpp>

namespace kloss {
namespace creator {

class new_block_tool : public tool
{
public:
    new_block_tool(gl_widget& parent);

    void mouse_press_event(QMouseEvent* event) override;

private:
    gl_widget& parent_;
};

} // namespace creator
} // namespace kloss
