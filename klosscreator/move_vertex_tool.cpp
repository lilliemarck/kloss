#include "move_vertex_tool.hpp"
#include <kloss/algorithm.hpp>
#include <klosscreator/main_window.hpp>
#include <klosscreator/vertex_array.hpp>

static float const snap_size = 1.0f / 8.0f;

move_vertex_tool::move_vertex_tool(main_window& window)
    : window_(window)
    , document_(window.get_document())
{
}

move_vertex_tool::~move_vertex_tool()
{
    document_.unlock();
}

void move_vertex_tool::mouse_pressed(ui_mouseevent const& event)
{
    if (event.button == UI_MOUSEBUTTON_LEFT)
    {
        auto pick = window_.pick_vertex(event.x, event.y);
        bool did_select;

        if (event.modifiers & UI_MODIFIER_CTRL)
        {
            did_select = multi_pick(selection_, pick.get_ptr());
        }
        else
        {
            did_select = single_pick(selection_, pick.get_ptr());
        }

        if (did_select)
        {
            reference_ = get_position(*pick);
            drag_backup_ = selection_.backup();

            document_.lock();
        }

        ui_update_widget(window_.get_gl_widget());
    }
}

void move_vertex_tool::mouse_released(ui_mouseevent const& event)
{
    if (event.button == UI_MOUSEBUTTON_LEFT)
    {
        document_.unlock();
        drag_backup_.reset();
    }
}

void move_vertex_tool::mouse_moved(ui_mouseevent const& event)
{
    if (drag_backup_)
    {
        ray ray = window_.make_mouse_ray(event.x, event.y);

        if (auto position = constrain(window_.get_constrain_mode(), ray, reference_))
        {
            vec3 translation = *position - reference_;

            translation.x = round_to_increment(translation.x, snap_size);
            translation.y = round_to_increment(translation.y, snap_size);
            translation.z = round_to_increment(translation.z, snap_size);

            selection_.restore(*drag_backup_);

            corner_ref *cornerrefs = selection_.get_corners();
            std::size_t count = selection_.corner_count();

            for (std::size_t i = 0; i < count; ++i)
            {
                translate(cornerrefs[i], translation);
            }

            document_.get_root_group()->update_vertex_array();
        }

        ui_update_widget(window_.get_gl_widget());
    }
}

void move_vertex_tool::draw()
{
    auto& vertices = window_.get_cursor_vertices();
    corner_ref const *cornerrefs = selection_.get_corners();
    std::size_t count = selection_.corner_count();

    for (std::size_t i = 0; i < count; ++i)
    {
        corner_ref const& ref = cornerrefs[i];

        if (ref.flags() & corner_ref::top)
        {
            draw_at(vertices, top(*ref));
        }

        if (ref.flags() & corner_ref::bottom)
        {
            draw_at(vertices, bottom(*ref));
        }
    }
}
