#pragma once

#include <ui/ui.h>

struct mainwindow;

typedef struct toolprocs toolprocs;

struct toolprocs
{
    void *(*create)(struct mainwindow *win);
    void (*destroy)(void *data);
    void (*key_pressed)(void *data, ui_key key);
    void (*key_released)(void *data, ui_key key);
    void (*mouse_pressed)(void *data, ui_mouseevent const *event);
    void (*mouse_released)(void *data, ui_mouseevent const *event);
    void (*mouse_moved)(void *data, ui_mouseevent const *event);
    void (*draw_gl)(void *data);
};

extern toolprocs moveblock_toolprocs;
extern toolprocs movecamera_toolprocs;
extern toolprocs movevertex_toolprocs;
extern toolprocs newblock_toolprocs;
extern toolprocs rotatecamera_toolprocs;
