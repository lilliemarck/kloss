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

typedef struct
{
    mainwindow *window;
    Document   *document;
    Vec3        reference;
    Buffer     *dragbackup;
} moveblock;

static void *create(mainwindow *win)
{
    moveblock *tool = calloc(1, sizeof(moveblock));

    tool->window = win;
    tool->document = get_document(win);

    return tool;
}

static void destroy(void *data)
{
    moveblock *tool = data;

    UnlockDocument(tool->document);
    DestroyBuffer(tool->dragbackup);
    DeselectAllBlocks(GetBlockSelection(tool->document));

    free(tool);
}

static void mouse_pressed(void *data, ui_mouseevent const *event)
{
    moveblock *tool = data;

    if (event->button == UI_MOUSEBUTTON_LEFT)
    {
        BlockSelection *selection = GetBlockSelection(tool->document);
        Ray mouse = make_mouseray(tool->window, event->x, event->y);
        Pick pick = PickGroupBlock(GetRootGroup(tool->document), &mouse);
        bool did_select;

        if (event->modifiers & UI_MODIFIER_CTRL)
        {
            did_select = MultiPickBlock(selection, pick.block);
        }
        else
        {
            did_select = SinglePickBlock(selection, pick.block);
        }

        if (did_select)
        {
            tool->reference = pick.intersection;

            DestroyBuffer(tool->dragbackup);
            tool->dragbackup = CreateBuffer();
            BackupBlockSelection(selection, tool->dragbackup);

            LockDocument(tool->document);
        }

        ui_update_widget(get_glwidget(tool->window));
    }
}

static void mouse_released(void *data, ui_mouseevent const *event)
{
    moveblock *tool = data;

    if (event->button == UI_MOUSEBUTTON_LEFT)
    {
        UnlockDocument(tool->document);
        DestroyBuffer(tool->dragbackup);
        tool->dragbackup = NULL;
    }
}

static void mouse_moved(void *data, ui_mouseevent const *event)
{
    moveblock *tool = data;

    if (tool->dragbackup)
    {
        Ray ray = make_mouseray(tool->window, event->x, event->y);
        Vec3 position;

        if (ConstrainRay(get_constrainalgorithm(tool->window), &ray, &tool->reference, &position))
        {
            BlockSelection *selection = GetBlockSelection(tool->document);
            RestoreBlockSelection(selection, tool->dragbackup);

            Vec3 translation;
            Vec3Subtract(&translation, &position, &tool->reference);

            translation.X = roundf(translation.X);
            translation.Y = roundf(translation.Y);
            translation.Z = roundf(translation.Z);

            Block** blocks = SelectedBlocks(selection);
            size_t count = SelectedBlockCount(selection);

            for (size_t i = 0; i < count; ++i)
            {
                TranslateBlock(blocks[i], &translation);
            }

            UpdateGroupVertexArray(GetRootGroup(tool->document));
        }

        ui_update_widget(get_glwidget(tool->window));
    }
}

static void draw_gl(void *data)
{
    moveblock *tool = data;

    Buffer *vertices = get_cursorvertices(tool->window);
    BlockSelection *selection = GetBlockSelection(tool->document);
    Block **blocks = SelectedBlocks(selection);
    size_t count = SelectedBlockCount(selection);

    for (size_t i = 0; i < count; ++i)
    {
        Block const *block = blocks[i];

        draw_at(vertices, (Vec3){block->Corners[0].X, block->Corners[0].Y, block->Corners[0].Top});
        draw_at(vertices, (Vec3){block->Corners[1].X, block->Corners[1].Y, block->Corners[1].Top});
        draw_at(vertices, (Vec3){block->Corners[2].X, block->Corners[2].Y, block->Corners[2].Top});
        draw_at(vertices, (Vec3){block->Corners[3].X, block->Corners[3].Y, block->Corners[3].Top});
        draw_at(vertices, (Vec3){block->Corners[0].X, block->Corners[0].Y, block->Corners[0].Bottom});
        draw_at(vertices, (Vec3){block->Corners[1].X, block->Corners[1].Y, block->Corners[1].Bottom});
        draw_at(vertices, (Vec3){block->Corners[2].X, block->Corners[2].Y, block->Corners[2].Bottom});
        draw_at(vertices, (Vec3){block->Corners[3].X, block->Corners[3].Y, block->Corners[3].Bottom});
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
