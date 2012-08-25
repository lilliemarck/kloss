#include "new_block_tool.hpp"
#include <memory>
#include <QMouseEvent>
#include <kloss/block.hpp>
#include <klosscreator/gl_widget.hpp>

namespace kloss {
namespace creator {

new_block_tool::new_block_tool(gl_widget& parent) : parent_(parent)
{
}

void new_block_tool::mouse_press_event(QMouseEvent const& event)
{
    if (event.button() == Qt::LeftButton)
    {
        if (cursor_position_)
        {
            float x      = cursor_position_->X;
            float y      = cursor_position_->Y;
            float top    = 1.0f;
            float bottom = 0.0f;

            block_ptr block = std::make_shared<kloss::block>();
            (*block)[0] = {x,        y,        top, bottom};
            (*block)[1] = {x + 1.0f, y,        top, bottom};
            (*block)[2] = {x + 1.0f, y + 1.0f, top, bottom};
            (*block)[3] = {x,        y + 1.0f, top, bottom};

            parent_.group().insert(block);
            parent_.update();
        }
    }
}

void new_block_tool::mouse_move_event(QMouseEvent const& event)
{
    cursor_position_ = intersect_xy_plane(parent_.mouse_ray(event.x(), event.y()));

    if (cursor_position_)
    {
        cursor_position_->X = std::round(cursor_position_->X);
        cursor_position_->Y = std::round(cursor_position_->Y);
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
