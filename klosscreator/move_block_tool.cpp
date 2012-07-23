#include "move_block_tool.hpp"
#include <QMouseEvent>
#include <kloss/block.hpp>
#include <klosscreator/gl_widget.hpp>

namespace kloss {
namespace creator {

move_block_tool::move_block_tool(gl_widget& parent) : parent_(parent)
{
}

void move_block_tool::mouse_press_event(QMouseEvent* event)
{
    block_ = parent_.world().pick(parent_.mouse_ray(event->x(), event->y()));
    parent_.update();
}

void move_block_tool::paint_gl()
{
    if (block_)
    {
        block const& block = *block_;
        auto const& cursor_vertices = parent_.cursor_vertices();
        draw(cursor_vertices, {block[0].x, block[0].y, block[0].top});
        draw(cursor_vertices, {block[1].x, block[1].y, block[1].top});
        draw(cursor_vertices, {block[2].x, block[2].y, block[2].top});
        draw(cursor_vertices, {block[3].x, block[3].y, block[3].top});
        draw(cursor_vertices, {block[0].x, block[0].y, block[0].bottom});
        draw(cursor_vertices, {block[1].x, block[1].y, block[1].bottom});
        draw(cursor_vertices, {block[2].x, block[2].y, block[2].bottom});
        draw(cursor_vertices, {block[3].x, block[3].y, block[3].bottom});
    }
}

} // namespace creator
} // namespace kloss
