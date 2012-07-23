#include "new_block_tool.hpp"
#include <QMouseEvent>
#include <kloss/block.hpp>
#include <klosscreator/gl_widget.hpp>

namespace kloss {
namespace creator {

new_block_tool::new_block_tool(gl_widget& parent) : parent_(parent)
{
}

void new_block_tool::mouse_press_event(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        if (cursor_position_)
        {
            float x      = (*cursor_position_)[0];
            float y      = (*cursor_position_)[1];
            float top    = 1.0f;
            float bottom = 0.0f;

            block block;
            block[0] = {x,        y,        top, bottom};
            block[1] = {x + 1.0f, y,        top, bottom};
            block[2] = {x + 1.0f, y + 1.0f, top, bottom};
            block[3] = {x,        y + 1.0f, top, bottom};

            parent_.world().insert(block);
            parent_.update();
        }
    }
}

void new_block_tool::mouse_move_event(QMouseEvent* event)
{
    cursor_position_ = intersect_xy_plane(parent_.mouse_ray(event->x(), event->y()));

    if (cursor_position_)
    {
        (*cursor_position_)[0] = std::round((*cursor_position_)[0]);
        (*cursor_position_)[1] = std::round((*cursor_position_)[1]);
    }

    parent_.update();
}

void new_block_tool::paint_gl()
{
    if (cursor_position_)
    {
        draw(parent_.cursor_vertices(), *cursor_position_);
    }
}

} // namespace creator
} // namespace kloss
