#include "tool.h"
#include <GL/gl.h>
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
    document *document;
    vec3 reference;
    struct blockcopy *dragbackup;
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
    destroy_blockcopy(tool->dragbackup);
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
        struct blockref *blockref = pick.blockref.block ? &pick.blockref : NULL;
        bool did_select;

        if (event->modifiers & UI_MODIFIER_CTRL)
        {
            did_select = multi_pick_block(selection, blockref);
        }
        else
        {
            did_select = single_pick_block(selection, blockref);
        }

        if (did_select)
        {
            destroy_blockcopy(tool->dragbackup);
            tool->reference = pick.intersection;
            tool->dragbackup = create_blockcopy(selection);
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
        destroy_blockcopy(tool->dragbackup);
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
            restore_blockcopy(selection, tool->dragbackup);

            vec3 translation;
            vec3_subtract(&translation, &position, &tool->reference);

            translation.x = roundf(translation.x);
            translation.y = roundf(translation.y);
            translation.z = roundf(translation.z);

            translate_blockselection(selection, translation);
        }

        ui_update_widget(get_glwidget(tool->window));
    }
}

static void draw_block(struct blockref *ref, struct vec3 const *pos, void *vertices)
{
    struct block *block = ref->block;

    glPushMatrix();
    glTranslatef(pos->x, pos->y, pos->z);

    draw_at(vertices, (vec3){block->corners[0].x, block->corners[0].y, block->corners[0].top});
    draw_at(vertices, (vec3){block->corners[1].x, block->corners[1].y, block->corners[1].top});
    draw_at(vertices, (vec3){block->corners[2].x, block->corners[2].y, block->corners[2].top});
    draw_at(vertices, (vec3){block->corners[3].x, block->corners[3].y, block->corners[3].top});
    draw_at(vertices, (vec3){block->corners[0].x, block->corners[0].y, block->corners[0].bottom});
    draw_at(vertices, (vec3){block->corners[1].x, block->corners[1].y, block->corners[1].bottom});
    draw_at(vertices, (vec3){block->corners[2].x, block->corners[2].y, block->corners[2].bottom});
    draw_at(vertices, (vec3){block->corners[3].x, block->corners[3].y, block->corners[3].bottom});

    glPopMatrix();
}

static void draw_gl(void *data)
{
    moveblock *tool = data;

    buffer *vertices = get_cursorvertices(tool->window);
    blockselection *selection = get_blockselection(tool->document);

    foreach_selected_block(selection, draw_block, vertices);
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
