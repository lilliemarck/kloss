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

typedef struct
{
    mainwindow      *window;
    Document        *document;
    CornerSelection *selection;
    Vec3             reference;
    Buffer          *dragbackup;
} movevertex;

static void *create(mainwindow *win)
{
    movevertex *tool = calloc(1, sizeof(movevertex));

    tool->window     = win;
    tool->document   = get_document(win);
    tool->selection  = CreateCornerSelection();
    tool->dragbackup = NULL;

    return tool;
}

static void destroy(void *data)
{
    movevertex *tool = data;

    UnlockDocument(tool->document);
    DestroyBuffer(tool->dragbackup);
    DestroyCornerSelection(tool->selection);

    free(tool);
}

static void mouse_pressed(void *data, ui_mouseevent const *event)
{
    movevertex *tool = data;

    if (event->button == UI_MOUSEBUTTON_LEFT)
    {
        CornerRef pick;
        bool did_pick = pick_vertex(tool->window, event->x, event->y, &pick);
        bool did_select;

        if (event->modifiers & UI_MODIFIER_CTRL)
        {
            did_select = MultiPickCorner(tool->selection, did_pick ? &pick : NULL);
        }
        else
        {
            did_select = SinglePickCorner(tool->selection, did_pick ? &pick : NULL);
        }

        if (did_select)
        {
            tool->reference = CornerRefPosition(&pick);

            DestroyBuffer(tool->dragbackup);
            tool->dragbackup = CreateBuffer();
            BackupCornerSelection(tool->selection, tool->dragbackup);

            LockDocument(tool->document);
        }

        ui_update_widget(get_glwidget(tool->window));
    }
}

static void mouse_released(void *data, ui_mouseevent const *event)
{
    movevertex *tool = data;

    if (event->button == UI_MOUSEBUTTON_LEFT)
    {
        UnlockDocument(tool->document);
        DestroyBuffer(tool->dragbackup);
        tool->dragbackup = NULL;
    }
}

static void mouse_moved(void *data, ui_mouseevent const *event)
{
    movevertex *tool = data;

    if (tool->dragbackup)
    {
        Ray ray = make_mouseray(tool->window, event->x, event->y);
        Vec3 position;

        if (ConstrainRay(get_constrainalgorithm(tool->window), &ray, &tool->reference, &position))
        {
            Vec3 translation;
            Vec3Subtract(&translation, &position, &tool->reference);

            translation.X = RoundToStepf(translation.X, snap_size);
            translation.Y = RoundToStepf(translation.Y, snap_size);
            translation.Z = RoundToStepf(translation.Z, snap_size);

            RestoreCornerSelection(tool->selection, tool->dragbackup);

            CornerRef *cornerrefs = SelectedCorners(tool->selection);
            size_t count = SelectedCornerCount(tool->selection);

            for (size_t i = 0; i < count; ++i)
            {
                TranslateCornerRef(cornerrefs + i, &translation);
            }

            UpdateGroupVertexArray(GetRootGroup(tool->document));
        }

        ui_update_widget(get_glwidget(tool->window));
    }
}

static void draw_gl(void *data)
{
    movevertex *tool = data;

    Buffer *vertices = get_cursorvertices(tool->window);
    CornerRef const *cornerrefs = SelectedCorners(tool->selection);
    size_t count = SelectedCornerCount(tool->selection);

    for (size_t i = 0; i < count; ++i)
    {
        CornerRef const* corner = cornerrefs + i;

        if (corner->Flags & CORNER_REF_TOP)
        {
            draw_at(vertices, CornerTop(corner->Corner));
        }

        if (corner->Flags & CORNER_REF_BOTTOM)
        {
            draw_at(vertices, CornerBottom(corner->Corner));
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
