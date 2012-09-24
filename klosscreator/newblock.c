#include "tool.h"
#include <kloss/block.h>
#include <kloss/group.h>
#include <klosscreator/document.h>
#include <klosscreator/mainwindow.h>
#include <klosscreator/vertexarray.h>
#include <math.h>
#include <stdlib.h>

typedef struct
{
    mainwindow *window;
    bool        hascursor;
    Vec3        cursor;
} newblock;

static void *create(mainwindow *win)
{
    newblock *tool = calloc(1, sizeof(newblock));

    tool->window = win;

    return tool;
}

static void mouse_pressed(void *data, ui_mouseevent const *event)
{
    newblock *tool = data;

    if (event->button == UI_MOUSEBUTTON_LEFT)
    {
        if (tool->hascursor)
        {
            float x      = tool->cursor.X;
            float y      = tool->cursor.Y;
            float top    = 1.0f;
            float bottom = 0.0f;

            Block *block = CreateBlock();
            block->Corners[0] = (Corner){x,        y,        top, bottom};
            block->Corners[1] = (Corner){x + 1.0f, y,        top, bottom};
            block->Corners[2] = (Corner){x + 1.0f, y + 1.0f, top, bottom};
            block->Corners[3] = (Corner){x,        y + 1.0f, top, bottom};

            Document *doc = get_document(tool->window);
            InsertBlocksInGroup(GetRootGroup(doc), &block, 1);
            ui_update_widget(get_glwidget(tool->window));
        }
    }
}

static void mouse_moved(void *data, ui_mouseevent const *event)
{
    newblock *tool = data;

    Ray ray = make_mouseray(tool->window, event->x, event->y);
    Vec3 pos;

    tool->hascursor = RayIntersectXYPlane(&pos, &ray, 0.0f);

    if (tool->hascursor)
    {
        tool->cursor.X = roundf(pos.X);
        tool->cursor.Y = roundf(pos.Y);
        // Don't bother with setting z to 0 because z is 0 since calloc
    }

    ui_update_widget(get_glwidget(tool->window));
}

static void draw_gl(void *data)
{
    newblock *tool = data;

    if (tool->hascursor)
    {
        draw_at(get_cursorvertices(tool->window), tool->cursor);
    }
}

toolprocs newblock_toolprocs =
{
    .create = create,
    .destroy = free,
    .mouse_pressed = mouse_pressed,
    .mouse_moved = mouse_moved,
    .draw_gl = draw_gl
};
