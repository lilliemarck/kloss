#include "new_block_tool.hpp"
#include <kloss/block.h>
#include <klosscreator/gl_widget.hpp>
#include <memory>

namespace kloss {
namespace creator {

new_block_tool::new_block_tool(gl_widget& parent) : parent_(parent)
{
}

void new_block_tool::mouse_press_event(ui_mouseevent const *event)
{
    if (event->button == UI_MOUSEBUTTON_LEFT)
    {
        if (cursor_position_)
        {
            float x      = cursor_position_->X;
            float y      = cursor_position_->Y;
            float top    = 1.0f;
            float bottom = 0.0f;

            Block *block = CreateBlock();
            block->Corners[0] = {x,        y,        top, bottom};
            block->Corners[1] = {x + 1.0f, y,        top, bottom};
            block->Corners[2] = {x + 1.0f, y + 1.0f, top, bottom};
            block->Corners[3] = {x,        y + 1.0f, top, bottom};

            InsertBlocksInGroup(parent_.group(), &block, 1);
            ui_update_widget(parent_.glwidget());
        }
    }
}

void new_block_tool::mouse_move_event(ui_mouseevent const *event)
{
    Ray mouse_ray = parent_.mouse_ray(event->x, event->y);
    Vec3 temp;

    if (RayIntersectXYPlane(&temp, &mouse_ray, 0.0f))
    {
        cursor_position_ = temp;
        cursor_position_->X = std::round(temp.X);
        cursor_position_->Y = std::round(temp.Y);
    }
    else
    {
        cursor_position_.reset();
    }

    ui_update_widget(parent_.glwidget());
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
