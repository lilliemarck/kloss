#include "ui.h"
#include <QAction>
#include <QActionGroup>
#include <QApplication>
#include <QGLWidget>
#include <QKeyEvent>
#include <QMenu>
#include <QMenuBar>
#include <QObject>
#include <QBasicTimer>
#include <QToolBar>
#include <ui/ui_qt.h>

extern "C" {

struct ui_menu
{
    ui_window *parent;
    QMenu *qmenu;
};

struct ui_toolbar
{
    ui_window *parent;
    QToolBar *qtoolbar;
};

struct ui_timer : public QObject, public QBasicTimer
{
    void timerEvent(QTimerEvent *event) override
    {
        proc(data, this);
    }

    ui_timerproc proc;
    void *data;
};

static ui_key map_key(QKeyEvent* event)
{
    switch (event->key())
    {
        default: return UI_KEY_RESERVED;

        case Qt::Key_A: return UI_KEY_A;
        case Qt::Key_B: return UI_KEY_B;
        case Qt::Key_C: return UI_KEY_C;
        case Qt::Key_D: return UI_KEY_D;
        case Qt::Key_E: return UI_KEY_E;
        case Qt::Key_F: return UI_KEY_F;
        case Qt::Key_G: return UI_KEY_G;
        case Qt::Key_H: return UI_KEY_H;
        case Qt::Key_I: return UI_KEY_I;
        case Qt::Key_J: return UI_KEY_J;
        case Qt::Key_K: return UI_KEY_K;
        case Qt::Key_L: return UI_KEY_L;
        case Qt::Key_M: return UI_KEY_M;
        case Qt::Key_N: return UI_KEY_N;
        case Qt::Key_O: return UI_KEY_O;
        case Qt::Key_P: return UI_KEY_P;
        case Qt::Key_Q: return UI_KEY_Q;
        case Qt::Key_R: return UI_KEY_R;
        case Qt::Key_S: return UI_KEY_S;
        case Qt::Key_T: return UI_KEY_T;
        case Qt::Key_U: return UI_KEY_U;
        case Qt::Key_V: return UI_KEY_V;
        case Qt::Key_W: return UI_KEY_W;
        case Qt::Key_X: return UI_KEY_X;
        case Qt::Key_Y: return UI_KEY_Y;
        case Qt::Key_Z: return UI_KEY_Z;
    }
}

static ui_mousebuttons map_mousebutton(Qt::MouseButton qbutton)
{
    switch (qbutton)
    {
        default: return ui_mousebuttons(0);

        case Qt::LeftButton:   return UI_MOUSEBUTTON_LEFT;
        case Qt::RightButton:  return UI_MOUSEBUTTON_RIGHT;
        case Qt::MiddleButton: return UI_MOUSEBUTTON_MIDDLE;
    };
}

static uint8_t map_modifiers(Qt::KeyboardModifiers qmodifier)
{
    uint8_t modifiers = 0;

    if (qmodifier & Qt::ShiftModifier)   modifiers |= UI_MODIFIER_SHIFT;
    if (qmodifier & Qt::ControlModifier) modifiers |= UI_MODIFIER_CTRL;
    if (qmodifier & Qt::AltModifier)     modifiers |= UI_MODIFIER_ALT;
    if (qmodifier & Qt::MetaModifier)    modifiers |= UI_MODIFIER_GUI;

    return modifiers;
}

static void map_event(ui_mouseevent *event, QMouseEvent const *qevent)
{
    event->x         = qevent->x();
    event->y         = qevent->y();
    event->button    = map_mousebutton(qevent->button());
    event->modifiers = map_modifiers(qevent->modifiers());
}

struct ui_glwidget : public QGLWidget
{
    ui_glwidget()
        : procs(nullptr)
        , data(nullptr)
        , keyprocs(nullptr)
        , keydata(nullptr)
        , mouseprocs(nullptr)
        , mousedata(nullptr)
    {
    }

    void keyPressEvent(QKeyEvent* qevent) override
    {
        if (keyprocs)
        {
            keyprocs->pressed(keydata, map_key(qevent));
        }
    }

    void keyReleaseEvent(QKeyEvent* qevent) override
    {
        if (keyprocs)
        {
            keyprocs->released(keydata, map_key(qevent));
        }
    }

    void mousePressEvent(QMouseEvent* qevent) override
    {
        if (mouseprocs)
        {
            ui_mouseevent event;
            map_event(&event, qevent);
            mouseprocs->pressed(mousedata, &event);
        }
    }

    void mouseReleaseEvent(QMouseEvent* qevent) override
    {
        if (mouseprocs)
        {
            ui_mouseevent event;
            map_event(&event, qevent);
            mouseprocs->released(mousedata, &event);
        }
    }

    void mouseMoveEvent(QMouseEvent* qevent) override
    {
        if (mouseprocs)
        {
            ui_mouseevent event;
            map_event(&event, qevent);
            mouseprocs->moved(mousedata, &event);
        }
    }

    void initializeGL() override
    {
        if (procs)
        {
            procs->init(data);
        }
    }

    void resizeGL(int width, int height) override
    {
        if (procs)
        {
            procs->resize(data, width, height);
        }
    }

    void paintGL() override
    {
        if (procs)
        {
            procs->draw(data);
        }
    }

    ui_glprocs const *procs;
    void *data;
    ui_keyprocs const *keyprocs;
    void *keydata;
    ui_mouseprocs const *mouseprocs;
    void *mousedata;
};

static void add_child(ui_window *win, void *data, void (*release)(void*))
{
    win->children.push_back({data, release});
}

ui_window *ui_create_window(void)
{
    return new ui_window;
}

void ui_destroy_window(ui_window *win)
{
    for (std::vector<Child>::iterator it = win->children.begin(); it != win->children.end(); ++it)
    {
        it->release(it->data);
    }

    delete win;
}

void ui_set_window_title(ui_window *win, char const *title)
{
    win->qwindow.setWindowTitle(title);
}

char const *ui_get_window_title(ui_window *win)
{
    win->string = win->qwindow.windowTitle().toUtf8();
    return win->string.constData();
}

void ui_resize_window(ui_window *win, int width, int height)
{
    win->qwindow.resize(width, height);
}

void ui_show_window(ui_window *win)
{
    win->qwindow.show();
}

void ui_close_window(ui_window *win)
{
    win->qwindow.close();
}

void ui_set_center_widget(ui_window *win, ui_glwidget *widget)
{
    win->qwindow.setCentralWidget(widget);
}

static void destroy_menu(void *data)
{
    delete static_cast<ui_menu*>(data);
}

ui_menu *ui_create_menu(ui_window *win)
{
    ui_menu *menu = new ui_menu;
    menu->parent = win;
    menu->qmenu = new QMenu(&win->qwindow);
    add_child(win, menu, destroy_menu);
    win->qwindow.menuBar()->addMenu(menu->qmenu);
    return menu;
}

void ui_set_menu_title(ui_menu *menu, char const *title)
{
    menu->qmenu->setTitle(title);
}

void ui_add_menu_action(ui_menu *menu, ui_action *action)
{
    menu->qmenu->addAction(action->qaction);
}

void ui_add_menu_separator(ui_menu *menu)
{
    menu->qmenu->addSeparator();
}

static void destroy_toolbar(void *data)
{
    delete static_cast<ui_toolbar*>(data);
}

ui_toolbar *ui_create_toolbar(ui_window *win)
{
    ui_toolbar *bar = new ui_toolbar;
    bar->parent = win;
    bar->qtoolbar = new QToolBar(&win->qwindow);
    add_child(win, bar, destroy_toolbar);
    win->qwindow.addToolBar(bar->qtoolbar);
    return bar;
}

void ui_set_toolbar_title(ui_toolbar *bar, char const *title)
{
    bar->qtoolbar->setWindowTitle(title);
}

void ui_add_toolbar_action(ui_toolbar *bar, ui_action *action)
{
    bar->qtoolbar->addAction(action->qaction);
}

void ui_add_toolbar_separator(ui_toolbar *bar)
{
    bar->qtoolbar->addSeparator();
}

static void destroy_action(void *data)
{
    delete static_cast<ui_action*>(data);
}

ui_action *ui_create_action(ui_window *win)
{
    ui_action *action = new ui_action(win);
    add_child(win, action, destroy_action);
    return action;
}

void ui_set_action_text(ui_action *action, char const *text)
{
    action->qaction->setText(text);
}

void ui_set_action_shortcut(ui_action *action, char const *shortcut)
{
    action->qaction->setShortcut(QString(shortcut));
}

void ui_set_action_checkable(ui_action *action, bool checkable)
{
    action->qaction->setCheckable(checkable);
}

void ui_set_actionproc(ui_action *action, ui_actionproc proc, void* data)
{
    action->proc = proc;
    action->data = data;
}

void ui_trig_action(ui_action *action)
{
    action->qaction->trigger();
}

static void destroy_actiongroup(void *data)
{
    delete static_cast<ui_actiongroup*>(data);
}

ui_actiongroup *ui_create_actiongroup(ui_window *win)
{
    ui_actiongroup *group = new ui_actiongroup(win);
    add_child(win, group, destroy_actiongroup);
    return group;
}

void ui_add_action_to_group(ui_actiongroup *group, ui_action *action)
{
    group->qactiongroup->addAction(action->qaction);
}

ui_glwidget *ui_create_glwidget(ui_glprocs const* procs, void *data)
{
    ui_glwidget *glw = new ui_glwidget;
    glw->setMouseTracking(true);
    glw->procs = procs;
    glw->data = data;
    return glw;
}

void ui_focus_widget(ui_glwidget *glw)
{
    glw->setFocus(Qt::OtherFocusReason);
}

int ui_widget_width(ui_glwidget const *glw)
{
    return glw->width();
}

int ui_widget_height(ui_glwidget const *glw)
{
    return glw->height();
}

void ui_update_widget(ui_glwidget *glw)
{
    glw->update();
}

void ui_set_keyprocs(ui_glwidget *glw, ui_keyprocs const *procs, void *data)
{
    glw->keyprocs = procs;
    glw->keydata = data;
}

void ui_set_mouseprocs(ui_glwidget *glw, ui_mouseprocs const *procs, void *data)
{
    glw->mouseprocs = procs;
    glw->mousedata = data;
}

ui_timer *ui_start_timer(int msec, ui_timerproc proc, void *data)
{
    ui_timer *timer = new ui_timer;
    timer->proc = proc;
    timer->data = data;
    timer->start(msec, timer);
    return timer;
}

void ui_stop_timer(ui_timer *timer)
{
    delete timer;
}

} // extern "C"
