#include "move_block_tool.hpp"
#include <QMouseEvent>
#include <kloss/block.hpp>
#include <klosscreator/gl_widget.hpp>

namespace kloss {
namespace creator {

move_block_tool::move_block_tool(gl_widget& parent) : parent_(parent)
{
}

void move_block_tool::mouse_press_event(QMouseEvent const& event)
{
    if (event.button() == Qt::LeftButton)
    {
        auto pick = parent_.world().pick(parent_.mouse_ray(event.x(), event.y()));

        block_ = pick.block;

        if (block_)
        {
            drag_ = {pick.intersection, *block_};
        }

        parent_.update();
    }
}

void move_block_tool::mouse_release_event(QMouseEvent const& event)
{
    if (event.button() == Qt::LeftButton)
    {
        drag_.reset();
    }
}

void move_block_tool::mouse_move_event(QMouseEvent const& event)
{
    if (drag_)
    {
        auto mouse_ray = parent_.mouse_ray(event.x(), event.y());
        auto position = constrain(parent_.get_constrain_algorithm(), mouse_ray, drag_->drag_origin);

        if (position)
        {
            cml::vector3f translation = *position - drag_->drag_origin;

            translation[0] = std::round(translation[0]);
            translation[1] = std::round(translation[1]);
            translation[2] = std::round(translation[2]);

            *block_ = drag_->original_block;

            for (auto& column : *block_)
            {
                column.x += translation[0];
                column.y += translation[1];
                column.top += translation[2];
                column.bottom += translation[2];
            }

            parent_.world().update_vertex_array();
        }

        parent_.update();
    }
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
