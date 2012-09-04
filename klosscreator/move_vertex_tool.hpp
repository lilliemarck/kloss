#pragma once

#include <boost/optional.hpp>
#include <kloss/block.h>
#include <klosscreator/cornerselection.h>
#include <klosscreator/document_lock.hpp>
#include <klosscreator/tool.hpp>

struct CornerSelection;

namespace kloss {
namespace creator {

class move_vertex_tool : public tool
{
public:
    move_vertex_tool(gl_widget& parent);
    ~move_vertex_tool();

    void mouse_press_event(ui_mouseevent const *event) override;
    void mouse_release_event(ui_mouseevent const *event) override;
    void mouse_move_event(ui_mouseevent const *event) override;
    void paint_gl() override;

private:
    gl_widget& parent_;
    CornerSelection* selection_;
    boost::optional<Vec3> reference_;
    Buffer* drag_;
    document_lock document_lock_;
};

} // namespace creator
} // namespace kloss
