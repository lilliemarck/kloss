#pragma once

#include <kloss/block.hpp>
#include <klosscreator/corner_selection.hpp>
#include <klosscreator/document_lock.hpp>
#include <klosscreator/tool.hpp>

namespace kloss {
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
    corner_selection selection_;
    boost::optional<Vec3> reference_;
    boost::optional<corner_selection::backup_type> drag_;
    document_lock document_lock_;
};

} // namespace creator
} // namespace kloss
