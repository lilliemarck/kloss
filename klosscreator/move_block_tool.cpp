#include "move_block_tool.hpp"
#include <QMouseEvent>
#include <kloss/algorithm.hpp>
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

        if (event.modifiers() & Qt::ControlModifier)
        {
            multi_select(pick);
        }
        else
        {
            single_select(pick);
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
        auto position = constrain(parent_.get_constrain_algorithm(), mouse_ray, drag_->reference);

        if (position)
        {
            cml::vector3f translation = *position - drag_->reference;

            translation[0] = std::round(translation[0]);
            translation[1] = std::round(translation[1]);
            translation[2] = std::round(translation[2]);

            drag_->restore_blocks(selection_);

            for (block_ptr const& block : selection_)
            {
                translate(*block, translation);
            }

            parent_.world().update_vertex_array();
        }

        parent_.update();
    }
}

void move_block_tool::paint_gl()
{
    auto const& cursor_vertices = parent_.cursor_vertices();

    for (block_ptr const& pblock : selection_)
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

void move_block_tool::single_select(pick const& pick)
{
    if (pick.block)
    {
        if (!contains(selection_, pick.block))
        {
            selection_.clear();
            selection_.push_back(pick.block);
        }

        drag_ = drag(pick.intersection, selection_);
    }
    else
    {
        selection_.clear();
    }
}

void move_block_tool::multi_select(pick const& pick)
{
    if (pick.block)
    {
        if (!contains(selection_, pick.block))
        {
            selection_.push_back(pick.block);
            drag_ = drag(pick.intersection, selection_);
        }
        else
        {
            remove(selection_, pick.block);
        }
    }
}

move_block_tool::drag::drag(cml::vector3f const& reference, std::vector<block_ptr> const& blocks)
    : reference(reference)
{
    original_blocks.reserve(blocks.size());

    for (block_ptr const& block : blocks)
    {
        original_blocks.push_back(*block);
    }
}

void move_block_tool::drag::restore_blocks(std::vector<block_ptr> const& blocks)
{
    assert(original_blocks.size() == blocks.size());

    auto out_iter = begin(blocks);

    for (block const& block : original_blocks)
    {
        **out_iter++ = block;
    }
}

} // namespace creator
} // namespace kloss
