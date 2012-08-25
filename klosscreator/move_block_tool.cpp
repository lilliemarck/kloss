#include "move_block_tool.hpp"
#include <QMouseEvent>
#include <kloss/algorithm.hpp>
#include <kloss/block.h>
#include <kloss/buffer.h>
#include <klosscreator/document.h>
#include <klosscreator/gl_widget.hpp>
#include <klosscreator/pick.h>

namespace kloss {
namespace creator {

move_block_tool::move_block_tool(gl_widget& parent)
    : parent_(parent)
    , document_(parent.document())
    , drag_(nullptr)
    , document_lock_(parent.document())
{
}

move_block_tool::~move_block_tool()
{
    DestroyBuffer(drag_);
    DeselectAllBlocks(GetBlockSelection(document_));
}

void move_block_tool::mouse_press_event(QMouseEvent const& event)
{
    if (event.button() == Qt::LeftButton)
    {
        BlockSelection* selection = GetBlockSelection(document_);
        Ray mouse = parent_.mouse_ray(event.x(), event.y());
        Pick pick = PickGroupBlock(GetRootGroup(document_), &mouse);
        bool did_select;

        if (event.modifiers() & Qt::ControlModifier)
        {
            did_select = MultiPickBlock(selection, pick.block);
        }
        else
        {
            did_select = SinglePickBlock(selection, pick.block);
        }

        if (did_select)
        {
            reference_ = pick.intersection;

            DestroyBuffer(drag_);
            drag_ = CreateBuffer();
            BackupBlockSelection(selection, drag_);

            document_lock_.lock();
        }

        parent_.update();
    }
}

void move_block_tool::mouse_release_event(QMouseEvent const& event)
{
    if (event.button() == Qt::LeftButton)
    {
        document_lock_.unlock();
        DestroyBuffer(drag_);
        drag_ = nullptr;
    }
}

void move_block_tool::mouse_move_event(QMouseEvent const& event)
{
    if (drag_)
    {
        auto mouse_ray = parent_.mouse_ray(event.x(), event.y());
        Vec3 position;

        if (ConstrainRay(parent_.get_constrain_algorithm(), &mouse_ray, reference_.get_ptr(), &position))
        {
            BlockSelection* selection = GetBlockSelection(document_);
            RestoreBlockSelection(selection, drag_);

            Vec3 translation;
            Vec3Subtract(&translation, &position, reference_.get_ptr());

            translation.X = std::round(translation.X);
            translation.Y = std::round(translation.Y);
            translation.Z = std::round(translation.Z);

            Block** blocks = SelectedBlocks(selection);
            size_t count = SelectedBlockCount(selection);

            for (size_t i = 0; i < count; ++i)
            {
                TranslateBlock(blocks[i], &translation);
            }

            UpdateGroupVertexArray(parent_.group());
        }

        parent_.update();
    }
}

void move_block_tool::paint_gl()
{
    auto const& cursor_vertices = parent_.cursor_vertices();
    BlockSelection* selection = GetBlockSelection(document_);
    Block** blocks = SelectedBlocks(selection);
    size_t count = SelectedBlockCount(selection);

    for (size_t i = 0; i < count; ++i)
    {
        Block const *block = blocks[i];

        draw(cursor_vertices, {block->Corners[0].X, block->Corners[0].Y, block->Corners[0].Top});
        draw(cursor_vertices, {block->Corners[1].X, block->Corners[1].Y, block->Corners[1].Top});
        draw(cursor_vertices, {block->Corners[2].X, block->Corners[2].Y, block->Corners[2].Top});
        draw(cursor_vertices, {block->Corners[3].X, block->Corners[3].Y, block->Corners[3].Top});
        draw(cursor_vertices, {block->Corners[0].X, block->Corners[0].Y, block->Corners[0].Bottom});
        draw(cursor_vertices, {block->Corners[1].X, block->Corners[1].Y, block->Corners[1].Bottom});
        draw(cursor_vertices, {block->Corners[2].X, block->Corners[2].Y, block->Corners[2].Bottom});
        draw(cursor_vertices, {block->Corners[3].X, block->Corners[3].Y, block->Corners[3].Bottom});
    }
}

} // namespace creator
} // namespace kloss
