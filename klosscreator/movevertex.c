#include "tool.h"
#include <kloss/algorithm.h>
#include <kloss/buffer.h>
#include <kloss/cornerref.h>
#include <kloss/group.h>
#include <klosscreator/cornerselection.h>
#include <klosscreator/document.h>
#include <klosscreator/mainwindow.h>
#include <klosscreator/pick.h>
#include <klosscreator/vertexarray.h>
#include <stdlib.h>

static float const snap_size = 1.0f / 8.0f;

typedef struct movevertex
{
    mainwindow      *window;
    document        *document;
    cornerselection *selection;
    vec3             reference;
    buffer          *dragbackup;
} movevertex;

static void *create(mainwindow *win)
{
    movevertex *tool = calloc(1, sizeof(struct movevertex));

    tool->window     = win;
    tool->document   = get_document(win);
    tool->selection  = create_cornerselection();
    tool->dragbackup = NULL;

    return tool;
}

static void destroy(void *data)
{
    movevertex *tool = data;

    unlock_document(tool->document);
    destroy_buffer(tool->dragbackup);
    destroy_cornerselection(tool->selection);

    free(tool);
}

static void mouse_pressed(void *data, ui_mouseevent const *event)
{
    movevertex *tool = data;

    if (event->button == UI_MOUSEBUTTON_LEFT)
    {
        cornerref pick;
        bool did_pick = pick_vertex(tool->window, event->x, event->y, &pick);
        bool did_select;

        if (event->modifiers & UI_MODIFIER_CTRL)
        {
            did_select = multi_pick_corner(tool->selection, did_pick ? &pick : NULL);
        }
        else
        {
            did_select = single_pick_corner(tool->selection, did_pick ? &pick : NULL);
        }

        if (did_select)
        {
            tool->reference = cornerref_position(&pick);

            destroy_buffer(tool->dragbackup);
            tool->dragbackup = create_buffer();
            backup_cornerselection(tool->selection, tool->dragbackup);

            lock_document(tool->document);
        }

        ui_update_widget(get_glwidget(tool->window));
    }
}

static void mouse_released(void *data, ui_mouseevent const *event)
{
    movevertex *tool = data;

    if (event->button == UI_MOUSEBUTTON_LEFT)
    {
        unlock_document(tool->document);
        destroy_buffer(tool->dragbackup);
        tool->dragbackup = NULL;
    }
}

static void mouse_moved(void *data, ui_mouseevent const *event)
{
    movevertex *tool = data;

    if (tool->dragbackup)
    {
        ray ray = make_mouseray(tool->window, event->x, event->y);
        vec3 position;

        if (constrain_ray(get_constrainalgorithm(tool->window), &ray, &tool->reference, &position))
        {
            vec3 translation;
            vec3_subtract(&translation, &position, &tool->reference);

            translation.x = roundtofractf(translation.x, snap_size);
            translation.y = roundtofractf(translation.y, snap_size);
            translation.z = roundtofractf(translation.z, snap_size);

            restore_cornerselection(tool->selection, tool->dragbackup);

            cornerref *cornerrefs = selected_corners(tool->selection);
            size_t count = selected_corner_count(tool->selection);

            for (size_t i = 0; i < count; ++i)
            {
                translate_cornerref(cornerrefs + i, &translation);
            }

            update_group_vertexarray(get_root_group(tool->document));
        }

        ui_update_widget(get_glwidget(tool->window));
    }
}

static void draw_gl(void *data)
{
    movevertex *tool = data;

    buffer *vertices = get_cursorvertices(tool->window);
    cornerref const *cornerrefs = selected_corners(tool->selection);
    size_t count = selected_corner_count(tool->selection);

    for (size_t i = 0; i < count; ++i)
    {
        cornerref const* corner = cornerrefs + i;

        if (corner->flags & CORNERREF_TOP)
        {
            draw_at(vertices, cornertop(corner->corner));
        }

        if (corner->flags & CORNERREF_BOTTOM)
        {
            draw_at(vertices, cornerbottom(corner->corner));
        }
    }
}

toolprocs movevertex_toolprocs =
{
    .create = create,
    .destroy = destroy,
    .mouse_pressed = mouse_pressed,
    .mouse_released = mouse_released,
    .mouse_moved = mouse_moved,
    .draw_gl = draw_gl
};
