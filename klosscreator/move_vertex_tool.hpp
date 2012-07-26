#pragma once

#include <cml/cml.h>
#include <kloss/block.hpp>
#include <klosscreator/tool.hpp>

namespace kloss {

struct pick;

namespace creator {

class move_vertex_tool : public tool
{
public:
    move_vertex_tool(gl_widget& parent);

    void mouse_press_event(QMouseEvent const& event) override;
    void mouse_release_event(QMouseEvent const& event) override;
    void mouse_move_event(QMouseEvent const& event) override;
    void paint_gl() override;

private:
    gl_widget& parent_;
    boost::optional<cml::vector3f> vertex_;
};

} // namespace creator
} // namespace kloss
