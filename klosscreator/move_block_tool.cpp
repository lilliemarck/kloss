#include "move_block_tool.hpp"
#include <GL/gl.h>
#include <klosscreator/main_window.hpp>
#include <klosscreator/vertex_array.hpp>

move_block_tool::move_block_tool(main_window& window)
    : window_(window)
    , document_(window.get_document())
{
}

move_block_tool::~move_block_tool()
{
    document_.unlock();
    document_.get_block_selection().deselect_all();
}

void move_block_tool::mouse_pressed(ui_mouseevent const& event)
{
    if (event.button == UI_MOUSEBUTTON_LEFT)
    {
        block_selection& selection = document_.get_block_selection();
        ray mouse = window_.make_mouse_ray(event.x, event.y);
        pick pick = pick_block(document_.get_root_group(), mouse);
        struct block_ref *blockref = pick.block_ref.block ? &pick.block_ref : NULL;
        bool did_select;

        if (event.modifiers & UI_MODIFIER_CTRL)
        {
            did_select = multi_pick(selection, blockref);
        }
        else
        {
            did_select = single_pick(selection, blockref);
        }

        if (did_select)
        {
            reference_ = pick.intersection;
            drag_backup_ = selection.copy();
            document_.lock();
        }

        ui_update_widget(window_.get_gl_widget());
    }
}

void move_block_tool::mouse_released(ui_mouseevent const& event)
{
    if (event.button == UI_MOUSEBUTTON_LEFT)
    {
        document_.unlock();
        drag_backup_.reset();
    }
}

void move_block_tool::mouse_moved(ui_mouseevent const& event)
{
    if (drag_backup_)
    {
        ray ray = window_.make_mouse_ray(event.x, event.y);

        if (auto position = constrain(window_.get_constrain_mode(), ray, reference_))
        {
            block_selection& selection = document_.get_block_selection();
            selection.restore(*drag_backup_);

            vec3 translation = *position - reference_;

            translation.x = std::round(translation.x);
            translation.y = std::round(translation.y);
            translation.z = std::round(translation.z);

            selection.translate(translation);
        }

        ui_update_widget(window_.get_gl_widget());
    }
}

void move_block_tool::draw()
{
    auto& vertices = window_.get_cursor_vertices();
    block_selection& selection = document_.get_block_selection();

    selection.for_each_block([&](block_ref& ref, vec3 const& position)
    {
        auto const& block = ref.block;

        glPushMatrix();
        glTranslatef(position.x, position.y, position.z);

        draw_at(vertices, {block->corners[0].x, block->corners[0].y, block->corners[0].top});
        draw_at(vertices, {block->corners[1].x, block->corners[1].y, block->corners[1].top});
        draw_at(vertices, {block->corners[2].x, block->corners[2].y, block->corners[2].top});
        draw_at(vertices, {block->corners[3].x, block->corners[3].y, block->corners[3].top});
        draw_at(vertices, {block->corners[0].x, block->corners[0].y, block->corners[0].bottom});
        draw_at(vertices, {block->corners[1].x, block->corners[1].y, block->corners[1].bottom});
        draw_at(vertices, {block->corners[2].x, block->corners[2].y, block->corners[2].bottom});
        draw_at(vertices, {block->corners[3].x, block->corners[3].y, block->corners[3].bottom});

        glPopMatrix();
    });
}
