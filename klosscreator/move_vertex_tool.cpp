#include "move_vertex_tool.hpp"
#include <QMouseEvent>
#include <kloss/algorithm.hpp>
#include <kloss/math.hpp>
#include <klosscreator/gl_widget.hpp>
#include <klosscreator/select_pick.hpp>

namespace kloss {
namespace creator {
namespace {

static float const snap_size = 1.0f / 8.0f;

} // namespace

move_vertex_tool::move_vertex_tool(gl_widget& parent)
    : parent_(parent)
    , document_lock_(parent.document())
{
}

void move_vertex_tool::mouse_press_event(QMouseEvent const& event)
{
    if (event.button() == Qt::LeftButton)
    {
        auto pick = parent_.pick_vertex(event.x(), event.y());
        bool did_select;

        if (event.modifiers() & Qt::ControlModifier)
        {
            did_select = multi_select(selection_, pick);
        }
        else
        {
            did_select = single_select(selection_, pick);
        }

        if (did_select)
        {
            reference_ = get_intersection(pick);
            drag_ = selection_.backup();
            document_lock_.lock();
        }

        parent_.update();
    }
}

void move_vertex_tool::mouse_release_event(QMouseEvent const& event)
{
    if (event.button() == Qt::LeftButton)
    {
        document_lock_.unlock();
        drag_.reset();
    }
}

void move_vertex_tool::mouse_move_event(QMouseEvent const& event)
{
    if (drag_)
    {
        auto mouse_ray = parent_.mouse_ray(event.x(), event.y());
        auto position = constrain(parent_.get_constrain_algorithm(), mouse_ray, *reference_);

        if (position)
        {
            cml::vector3f translation = *position - *reference_;

            translation[0] = round(translation[0], snap_size);
            translation[1] = round(translation[1], snap_size);
            translation[2] = round(translation[2], snap_size);

            selection_.restore(*drag_);

            for (auto& element : selection_)
            {
                element += translation;
            }

            parent_.group().update_vertex_array();
        }

        parent_.update();
    }
}

void move_vertex_tool::paint_gl()
{
    auto const& cursor_vertices = parent_.cursor_vertices();

    for (auto const& corner : selection_)
    {
        if (corner.flags() & corner_ref::top_flag)
        {
            draw(cursor_vertices, top(*corner));
        }

        if (corner.flags() & corner_ref::bottom_flag)
        {
            draw(cursor_vertices, bottom(*corner));
        }
    }
}

} // namespace creator
} // namespace kloss
