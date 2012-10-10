#include "new_block_tool.hpp"
#include <kloss/block.hpp>
#include <klosscreator/document.hpp>
#include <klosscreator/main_window.hpp>
#include <klosscreator/vertex_array.hpp>

new_block_tool::new_block_tool(main_window& window)
    : window_(window)
{
}

void new_block_tool::mouse_pressed(ui_mouseevent const& event)
{
    if (event.button == UI_MOUSEBUTTON_LEFT)
    {
        if (cursor_)
        {
            float x      = cursor_->x;
            float y      = cursor_->y;
            float top    = 1.0f;
            float bottom = 0.0f;

            block_ptr block = std::make_shared<struct block>();
            block->corners[0] = {x,        y,        top, bottom};
            block->corners[1] = {x + 1.0f, y,        top, bottom};
            block->corners[2] = {x + 1.0f, y + 1.0f, top, bottom};
            block->corners[3] = {x,        y + 1.0f, top, bottom};

            document& doc = window_.get_document();
            doc.get_root_group()->insert(&block, 1);
            ui_update_widget(window_.get_gl_widget());
        }
    }
}

void new_block_tool::mouse_moved(ui_mouseevent const& event)
{
    ray ray = window_.make_mouse_ray(event.x, event.y);

    if (cursor_ = intersect_xy_plane(ray, 0.0f))
    {
        cursor_->x = std::round(cursor_->x);
        cursor_->y = std::round(cursor_->y);
    }

    ui_update_widget(window_.get_gl_widget());
}

void new_block_tool::draw()
{
    if (cursor_)
    {
        draw_at(window_.get_cursor_vertices(), *cursor_);
    }
}
