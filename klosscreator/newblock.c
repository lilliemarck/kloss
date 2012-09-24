#include "tool.h"
#include <kloss/block.h>
#include <kloss/group.h>
#include <klosscreator/document.h>
#include <klosscreator/mainwindow.h>
#include <klosscreator/vertexarray.h>
#include <math.h>
#include <stdlib.h>

typedef struct newblock
{
    mainwindow *window;
    bool        hascursor;
    vec3        cursor;
} newblock;

static void *create(mainwindow *win)
{
    newblock *tool = calloc(1, sizeof(struct newblock));

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
            float x      = tool->cursor.x;
            float y      = tool->cursor.y;
            float top    = 1.0f;
            float bottom = 0.0f;

            block *block = create_block();
            block->corners[0] = (corner){x,        y,        top, bottom};
            block->corners[1] = (corner){x + 1.0f, y,        top, bottom};
            block->corners[2] = (corner){x + 1.0f, y + 1.0f, top, bottom};
            block->corners[3] = (corner){x,        y + 1.0f, top, bottom};

            document *doc = get_document(tool->window);
            insert_blocks(get_root_group(doc), &block, 1);
            ui_update_widget(get_glwidget(tool->window));
        }
    }
}

static void mouse_moved(void *data, ui_mouseevent const *event)
{
    newblock *tool = data;

    ray ray = make_mouseray(tool->window, event->x, event->y);
    vec3 pos;

    tool->hascursor = ray_intersect_xyplane(&pos, &ray, 0.0f);

    if (tool->hascursor)
    {
        tool->cursor.x = roundf(pos.x);
        tool->cursor.y = roundf(pos.y);
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
