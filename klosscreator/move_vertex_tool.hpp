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
    void single_select(boost::optional<vertex_ref> const& pick);
    void multi_select(boost::optional<vertex_ref> const& pick);

    struct drag
    {
        drag(cml::vector3f const& reference, std::vector<vertex_ref> const& vertices);
        void restore(std::vector<vertex_ref>& vertices);

        cml::vector3f reference;
        std::vector<corner> original_corners;
    };

    gl_widget& parent_;
    std::vector<vertex_ref> selection_;
    boost::optional<drag> drag_;
};

} // namespace creator
} // namespace kloss
