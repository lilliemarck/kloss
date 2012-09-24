#include "tool.h"
#include <kloss/camera.h>
#include <kloss/keypair.h>
#include <klosscreator/mainwindow.h>
#include <stdlib.h>

float const deltatime = 1.0f / 10.0f;

typedef struct movecamera
{
    mainwindow *window;
    keypair    *backward_forward;
    keypair    *left_right;
    ui_timer   *timer;
} movecamera;

static void update_keypair(keypair* keypair, bool pressed, ui_key event, ui_key first, ui_key second)
{
    if (event == first)
    {
        set_keypair_first(keypair, pressed);
    }
    else if (event == second)
    {
        set_keypair_second(keypair, pressed);
    }
}

static void on_timer(void* data, ui_timer* timer)
{
    movecamera *tool = data;

    if (timer == tool->timer)
    {
        camera* camera = get_camera(tool->window);
        move_camera_forward(camera, keypair_value(tool->backward_forward) * deltatime);
        move_camera_sideways(camera, keypair_value(tool->left_right) * deltatime);
        ui_update_widget(get_glwidget(tool->window));
    }
}

static void *create(mainwindow *win)
{
    movecamera *tool = calloc(1, sizeof(struct movecamera));

    tool->window           = win;
    tool->backward_forward = create_keypair();
    tool->left_right       = create_keypair();

    return tool;
}

static void destroy(void *data)
{
    movecamera *tool = data;

    ui_stop_timer(tool->timer);
    destroy_keypair(tool->backward_forward);
    destroy_keypair(tool->left_right);

    free(tool);
}

static void key_pressed(void *data, ui_key key)
{
    movecamera *tool = data;

    update_keypair(tool->backward_forward, true, key, UI_KEY_S, UI_KEY_W);
    update_keypair(tool->left_right,       true, key, UI_KEY_A, UI_KEY_D);

    if (keypair_value(tool->backward_forward) != 0.0f ||
        keypair_value(tool->left_right)       != 0.0f)
    {
        if (!tool->timer)
        {
            tool->timer = ui_start_timer(1.0f / deltatime, on_timer, tool);
        }
    }
}

static void key_released(void *data, ui_key key)
{
    movecamera *tool = data;

    update_keypair(tool->backward_forward, false, key, UI_KEY_S, UI_KEY_W);
    update_keypair(tool->left_right,       false, key, UI_KEY_A, UI_KEY_D);

    if (keypair_value(tool->backward_forward) == 0.0f &&
        keypair_value(tool->left_right)       == 0.0f)
    {
        ui_stop_timer(tool->timer);
        tool->timer = NULL;
    }
}

toolprocs movecamera_toolprocs =
{
    .create = create,
    .destroy = destroy,
    .key_pressed = key_pressed,
    .key_released = key_released
};
