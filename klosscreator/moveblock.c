#include "tool.h"
#include <kloss/block.h>
#include <kloss/buffer.h>
#include <kloss/group.h>
#include <klosscreator/blockselection.h>
#include <klosscreator/document.h>
#include <klosscreator/mainwindow.h>
#include <klosscreator/pick.h>
#include <klosscreator/vertexarray.h>
#include <math.h>
#include <stdlib.h>

typedef struct moveblock
{
    mainwindow *window;
    document   *document;
    vec3        reference;
    buffer     *dragbackup;
} moveblock;

static void *create(mainwindow *win)
{
    moveblock *tool = calloc(1, sizeof(struct moveblock));

    tool->window = win;
    tool->document = get_document(win);

    return tool;
}

static void destroy(void *data)
{
    moveblock *tool = data;

    unlock_document(tool->document);
    destroy_buffer(tool->dragbackup);
    deselect_all_blocks(get_blockselection(tool->document));

    free(tool);
}

static void mouse_pressed(void *data, ui_mouseevent const *event)
{
    moveblock *tool = data;

    if (event->button == UI_MOUSEBUTTON_LEFT)
    {
        blockselection *selection = get_blockselection(tool->document);
        ray mouse = make_mouseray(tool->window, event->x, event->y);
        pick pick = pick_block(get_root_group(tool->document), &mouse);
        bool did_select;

        if (event->modifiers & UI_MODIFIER_CTRL)
        {
            did_select = multi_pick_block(selection, pick.block);
        }
        else
        {
            did_select = single_pick_block(selection, pick.block);
        }

        if (did_select)
        {
            tool->reference = pick.intersection;

            destroy_buffer(tool->dragbackup);
            tool->dragbackup = create_buffer();
            backup_blockselection(selection, tool->dragbackup);

            lock_document(tool->document);
        }

        ui_update_widget(get_glwidget(tool->window));
    }
}

static void mouse_released(void *data, ui_mouseevent const *event)
{
    moveblock *tool = data;

    if (event->button == UI_MOUSEBUTTON_LEFT)
    {
        unlock_document(tool->document);
        destroy_buffer(tool->dragbackup);
        tool->dragbackup = NULL;
    }
}

static void mouse_moved(void *data, ui_mouseevent const *event)
{
    moveblock *tool = data;

    if (tool->dragbackup)
    {
        ray ray = make_mouseray(tool->window, event->x, event->y);
        vec3 position;

        if (constrain_ray(get_constrainalgorithm(tool->window), &ray, &tool->reference, &position))
        {
            blockselection *selection = get_blockselection(tool->document);
            restore_blockselection(selection, tool->dragbackup);

            vec3 translation;
            vec3_subtract(&translation, &position, &tool->reference);

            translation.x = roundf(translation.x);
            translation.y = roundf(translation.y);
            translation.z = roundf(translation.z);

            block** blocks = selected_blocks(selection);
            size_t count = selected_block_count(selection);

            for (size_t i = 0; i < count; ++i)
            {
                translate_block(blocks[i], &translation);
            }

            update_group_vertexarray(get_root_group(tool->document));
        }

        ui_update_widget(get_glwidget(tool->window));
    }
}

static void draw_gl(void *data)
{
    moveblock *tool = data;

    buffer *vertices = get_cursorvertices(tool->window);
    blockselection *selection = get_blockselection(tool->document);
    block **blocks = selected_blocks(selection);
    size_t count = selected_block_count(selection);

    for (size_t i = 0; i < count; ++i)
    {
        block const *block = blocks[i];

        draw_at(vertices, (vec3){block->corners[0].x, block->corners[0].y, block->corners[0].top});
        draw_at(vertices, (vec3){block->corners[1].x, block->corners[1].y, block->corners[1].top});
        draw_at(vertices, (vec3){block->corners[2].x, block->corners[2].y, block->corners[2].top});
        draw_at(vertices, (vec3){block->corners[3].x, block->corners[3].y, block->corners[3].top});
        draw_at(vertices, (vec3){block->corners[0].x, block->corners[0].y, block->corners[0].bottom});
        draw_at(vertices, (vec3){block->corners[1].x, block->corners[1].y, block->corners[1].bottom});
        draw_at(vertices, (vec3){block->corners[2].x, block->corners[2].y, block->corners[2].bottom});
        draw_at(vertices, (vec3){block->corners[3].x, block->corners[3].y, block->corners[3].bottom});
    }
}

toolprocs moveblock_toolprocs =
{
    .create = create,
    .destroy = destroy,
    .mouse_pressed = mouse_pressed,
    .mouse_released = mouse_released,
    .mouse_moved = mouse_moved,
    .draw_gl = draw_gl
};
