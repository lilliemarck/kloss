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
        if (auto position = parent_.cursor_position())
        {
            float x      = (*position)[0];
            float y      = (*position)[1];
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

} // namespace creator
} // namespace kloss
