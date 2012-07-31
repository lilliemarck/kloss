#include "move_block_tool.hpp"
#include <QMouseEvent>
#include <kloss/algorithm.hpp>
#include <kloss/block.hpp>
#include <klosscreator/document.hpp>
#include <klosscreator/gl_widget.hpp>
#include <klosscreator/select_pick.hpp>

namespace kloss {
namespace creator {

move_block_tool::move_block_tool(gl_widget& parent)
    : parent_(parent)
    , document_(parent.document())
{
}

void move_block_tool::mouse_press_event(QMouseEvent const& event)
{
    if (event.button() == Qt::LeftButton)
    {
        auto pick = document_.world.pick_block(parent_.mouse_ray(event.x(), event.y()));
        bool did_select;

        if (event.modifiers() & Qt::ControlModifier)
        {
            did_select = multi_select(document_.block_selection, pick);
        }
        else
        {
            did_select = single_select(document_.block_selection, pick);
        }

        if (did_select)
        {
            reference_ = get_intersection(pick);
            drag_ = document_.block_selection.backup();
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
        auto position = constrain(parent_.get_constrain_algorithm(), mouse_ray, *reference_);

        if (position)
        {
            cml::vector3f translation = *position - *reference_;

            translation[0] = std::round(translation[0]);
            translation[1] = std::round(translation[1]);
            translation[2] = std::round(translation[2]);

            document_.block_selection.restore(*drag_);

            for (auto& element : document_.block_selection)
            {
                translate(*element, translation);
            }

            parent_.world().update_vertex_array();
        }

        parent_.update();
    }
}

void move_block_tool::paint_gl()
{
    auto const& cursor_vertices = parent_.cursor_vertices();

    for (block_ptr const& pblock : document_.block_selection)
    {
        block const& block = *pblock;

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
