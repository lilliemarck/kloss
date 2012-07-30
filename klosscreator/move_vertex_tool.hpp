#pragma once

#include <cml/cml.h>
#include <kloss/block.hpp>
#include <klosscreator/selection.hpp>
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
    struct selection_policy
    {
        using pick_type = boost::optional<vertex_ref>;
        using selection_type = vertex_ref;
        using backup_type = corner;

        static bool const has_selection(pick_type const& pick)
        {
            return pick;
        }

        static selection_type const& get_selection(pick_type const& pick)
        {
            return *pick;
        }

        static cml::vector3f const get_intersection(pick_type const& pick)
        {
            return pick->to_vector();
        }

        static backup_type const& backup(selection_type const& selection)
        {
            return selection.corner();
        }

        static void restore(selection_type& selection, backup_type const& backup)
        {
            selection.corner() = backup;
        }
    };

    using selection_type = selection<selection_policy>;
    using backup_type = decltype(selection_type().backup());

    gl_widget& parent_;
    selection_type selection_;
    boost::optional<backup_type> drag_;
};

} // namespace creator
} // namespace kloss
