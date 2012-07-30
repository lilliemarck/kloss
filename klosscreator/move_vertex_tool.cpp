#include "move_vertex_tool.hpp"
#include <QMouseEvent>
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
    }
}

void move_vertex_tool::mouse_release_event(QMouseEvent const& event)
{
    if (event.button() == Qt::LeftButton)
    {
    }
}

void move_vertex_tool::mouse_move_event(QMouseEvent const& event)
{
    auto new_vertex = parent_.pick_vertex(event.x(), event.y());

    if (vertex_ != new_vertex)
    {
        vertex_ = new_vertex;
        parent_.update();
    }
}

void move_vertex_tool::paint_gl()
{
    if (vertex_)
    {
        auto const& cursor_vertices = parent_.cursor_vertices();
        draw(cursor_vertices, vertex_->to_vector());
    }
}

} // namespace creator
} // namespace kloss
