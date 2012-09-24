#include "tool.h"
#include <kloss/algorithm.h>
#include <kloss/camera.h>
#include <klosscreator/mainwindow.h>
#include <stdlib.h>

typedef struct rotatecamera
{
    mainwindow *window;
    bool        isdragging;
    int         mouseoriginx;
    int         mouseoriginy;
} rotatecamera;

static void *create(mainwindow *win)
{
    rotatecamera *tool = calloc(1, sizeof(rotatecamera));

    tool->window = win;

    return tool;
}

static void mouse_pressed(void *data, ui_mouseevent const *event)
{
    rotatecamera *tool = data;

    if (event->button == UI_MOUSEBUTTON_RIGHT)
    {
        tool->mouseoriginx = event->x;
        tool->mouseoriginy = event->y;
        tool->isdragging   = true;
    }
}

static void mouse_released(void *data, ui_mouseevent const *event)
{
    rotatecamera *tool = data;

    if (event->button == UI_MOUSEBUTTON_RIGHT)
    {
        tool->isdragging = false;
    }
}

static float minor_size(ui_glwidget const* widget)
{
    return Mini(ui_widget_width(widget), ui_widget_height(widget));
}

static void mouse_moved(void *data, ui_mouseevent const *event)
{
    rotatecamera *tool = data;

    if (tool->isdragging)
    {
        Camera *camera      = get_camera(tool->window);
        float   rotx        = event->x - tool->mouseoriginx;
        float   roty        = event->y - tool->mouseoriginy;
        float   degperpixel = 1.0f / minor_size(get_glwidget(tool->window));

        RotateCameraYaw   (camera, -rotx * degperpixel);
        RotateCameraPitch (camera, -roty * degperpixel);

        tool->mouseoriginx = event->x;
        tool->mouseoriginy = event->y;

        ui_update_widget(get_glwidget(tool->window));
    }
}

toolprocs rotatecamera_toolprocs =
{
    .create = create,
    .destroy = free,
    .mouse_pressed = mouse_pressed,
    .mouse_released = mouse_released,
    .mouse_moved = mouse_moved
};
