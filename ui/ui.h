#pragma once

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * Inspired by the USB HID standard.
 */
enum ui_modifiers
{
    UI_MODIFIER_CTRL  = 1,
    UI_MODIFIER_SHIFT = 2,
    UI_MODIFIER_ALT   = 4,
    UI_MODIFIER_GUI   = 8
};

enum ui_mousebuttons
{
    UI_MOUSEBUTTON_LEFT   = 1,
    UI_MOUSEBUTTON_RIGHT  = 2,
    UI_MOUSEBUTTON_MIDDLE = 4,
};

/**
 * Inspired by the USB HID standard.
 */
enum ui_key
{
    UI_KEY_RESERVED = 0,
    UI_KEY_A = 4,
    UI_KEY_B,
    UI_KEY_C,
    UI_KEY_D,
    UI_KEY_E,
    UI_KEY_F,
    UI_KEY_G,
    UI_KEY_H,
    UI_KEY_I,
    UI_KEY_J,
    UI_KEY_K,
    UI_KEY_L,
    UI_KEY_M,
    UI_KEY_N,
    UI_KEY_O,
    UI_KEY_P,
    UI_KEY_Q,
    UI_KEY_R,
    UI_KEY_S,
    UI_KEY_T,
    UI_KEY_U,
    UI_KEY_V,
    UI_KEY_W,
    UI_KEY_X,
    UI_KEY_Y,
    UI_KEY_Z,
};

typedef enum ui_mousebuttons ui_mousebuttons;
typedef enum ui_key ui_key;
typedef struct ui_action ui_action;
typedef struct ui_actiongroup ui_actiongroup;
typedef struct ui_glprocs ui_glprocs;
typedef struct ui_glwidget ui_glwidget;
typedef struct ui_keyprocs ui_keyprocs;
typedef struct ui_mouseevent ui_mouseevent;
typedef struct ui_mouseprocs ui_mouseprocs;
typedef struct ui_menu ui_menu;
typedef struct ui_timer ui_timer;
typedef struct ui_toolbar ui_toolbar;
typedef struct ui_window ui_window;

typedef void (*ui_actionproc)(void *data);
typedef void (*ui_timerproc)(void *data, ui_timer *timer);

struct ui_glprocs
{
    void (*init)(void *data);
    void (*resize)(void *data, int width, int height);
    void (*draw)(void *data);
};

struct ui_keyprocs
{
    void (*pressed)(void *data, ui_key key);
    void (*released)(void *data, ui_key key);
};

struct ui_mouseevent
{
    int x, y;
    ui_mousebuttons button;
    uint8_t modifiers;
};

struct ui_mouseprocs
{
    void (*pressed)(void *data, ui_mouseevent const *event);
    void (*released)(void *data, ui_mouseevent const *event);
    void (*moved)(void *data, ui_mouseevent const *event);
};

ui_window *ui_create_window(void);
void ui_destroy_window(ui_window *win);
void ui_set_window_title(ui_window *win, char const *title);
char const *ui_get_window_title(ui_window *win);
void ui_resize_window(ui_window *win, int width, int height);
void ui_show_window(ui_window *win);
void ui_close_window(ui_window *win);
void ui_set_center_widget(ui_window *win, ui_glwidget *widget);

ui_menu *ui_create_menu(ui_window *win);
void ui_set_menu_title(ui_menu *menu, char const *title);
void ui_add_menu_action(ui_menu *menu, ui_action *action);
void ui_add_menu_separator(ui_menu *menu);

ui_toolbar *ui_create_toolbar(ui_window *win);
void ui_set_toolbar_title(ui_toolbar *bar, char const *title);
void ui_add_toolbar_action(ui_toolbar *bar, ui_action *action);
void ui_add_toolbar_separator(ui_toolbar *bar);

ui_action *ui_create_action(ui_window *win);
void ui_set_action_text(ui_action *action, char const *text);
void ui_set_action_shortcut(ui_action *action, char const *shortcut);
void ui_set_action_checkable(ui_action *action, bool checkable);
void ui_set_actionproc(ui_action *action, ui_actionproc proc, void* data);
void ui_trig_action(ui_action *action);

ui_actiongroup *ui_create_actiongroup(ui_window *win);
void ui_add_action_to_group(ui_actiongroup *group, ui_action *action);

ui_glwidget *ui_create_glwidget(ui_glprocs const* procs, void *data);
void ui_focus_widget(ui_glwidget *glw);
int ui_widget_width(ui_glwidget const *glw);
int ui_widget_height(ui_glwidget const *glw);
void ui_update_widget(ui_glwidget *glw);
void ui_set_keyprocs(ui_glwidget *glw, ui_keyprocs const *procs, void *data);
void ui_set_mouseprocs(ui_glwidget *glw, ui_mouseprocs const *procs, void *data);

ui_timer *ui_start_timer(int msec, ui_timerproc proc, void *data);
void ui_stop_timer(ui_timer *timer);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus
