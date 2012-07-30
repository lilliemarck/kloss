#include "move_vertex_tool.hpp"
#include <QMouseEvent>
#include <kloss/algorithm.hpp>
#include <kloss/math.hpp>
#include <klosscreator/gl_widget.hpp>

namespace kloss {
namespace creator {

move_vertex_tool::move_vertex_tool(gl_widget& parent) : parent_(parent)
{
}

void move_vertex_tool::mouse_press_event(QMouseEvent const& event)
{
    if (event.button() == Qt::LeftButton)
    {
        auto pick = parent_.pick_vertex(event.x(), event.y());

        if (event.modifiers() & Qt::ControlModifier)
        {
            selection_.multi_select(pick);
        }
        else
        {
            selection_.single_select(pick);
        }

        if (selection_.reference())
        {
            drag_ = selection_.backup();
        }

        parent_.update();
    }
}

void move_vertex_tool::mouse_release_event(QMouseEvent const& event)
{
    if (event.button() == Qt::LeftButton)
    {
        drag_.reset();
    }
}

void move_vertex_tool::mouse_move_event(QMouseEvent const& event)
{
    if (drag_)
    {
        auto mouse_ray = parent_.mouse_ray(event.x(), event.y());
        auto position = constrain(parent_.get_constrain_algorithm(), mouse_ray, *selection_.reference());

        if (position)
        {
            cml::vector3f translation = *position - *selection_.reference();

            translation[0] = std::round(translation[0]);
            translation[1] = std::round(translation[1]);
            translation[2] = std::round(translation[2]);

            selection_.restore(*drag_);

            for (auto& element : selection_)
            {
                element.translate(translation);
            }

            parent_.world().update_vertex_array();
        }

        parent_.update();
    }
}

void move_vertex_tool::paint_gl()
{
    auto const& cursor_vertices = parent_.cursor_vertices();

    for (auto const& vertex : selection_)
    {
        draw(cursor_vertices, vertex.to_vector());
    }
}

} // namespace creator
} // namespace kloss
