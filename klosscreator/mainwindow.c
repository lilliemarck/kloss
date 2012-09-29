#include "mainwindow.h"
#include <GL/glu.h>
#include <kloss/buffer.h>
#include <kloss/camera.h>
#include <kloss/group.h>
#include <klosscreator/document.h>
#include <klosscreator/tool.h>
#include <klosscreator/vertexarray.h>
#include <ui/ui.h>
#include <stdlib.h>

struct mainwindow
{
    document* document;
    ui_window* window;
    ui_glwidget* glwidget;
    camera *camera;
    buffer *gridvertices;
    buffer *cursorvertices;
    constraint constraint;
    void *movecameratool;
    void *rotatecameratool;
    toolprocs const *toolprocs;
    void *tooldata;
    ui_action* quit_action;
    ui_action* cut_action;
    ui_action* copy_action;
    ui_action* paste_action;
    ui_action* delete_action;
    ui_action* group_action;
    ui_action* ungroup_action;
    ui_action* new_block_action;
    ui_action* move_block_action;
    ui_action* move_vertex_action_;
    ui_action* xyplane_constraint_action;
    ui_action* zaxis_constraint_action;
    ui_menu* filemenu;
    ui_menu* editmenu;
    ui_toolbar* toolbar;
};

static void close(void* data)
{
    mainwindow* self = data;
    ui_close_window(self->window);
}

static void cut(void* data)
{
    mainwindow* self = data;
    copy_selected_blocks(self->document);
    delete_selected_blocks(self->document);
    ui_update_widget(self->glwidget);
}

static void copy(void* data)
{
    mainwindow* self = data;
    copy_selected_blocks(self->document);
}

static void paste(void* data)
{
    mainwindow* self = data;
    paste_copied_blocks(self->document);
    ui_update_widget(self->glwidget);
}

static void del(void* data)
{
    mainwindow* self = data;
    delete_selected_blocks(self->document);
    ui_update_widget(self->glwidget);
}

static void group_action(void* data)
{
    mainwindow* self = data;
    group_selected_blocks(self->document);
    ui_update_widget(self->glwidget);
}

static void ungroup(void* data)
{
}

static void free_tool(mainwindow *win)
{
    if (win->toolprocs)
    {
        win->toolprocs->destroy(win->tooldata);
        win->toolprocs = NULL;
    }
}

static void use_tool(mainwindow *win, toolprocs const *procs)
{
    free_tool(win);

    win->toolprocs = procs;
    win->tooldata  = procs->create(win);

    ui_update_widget(win->glwidget);
}

static void use_new_block_tool(void* data)
{
    use_tool(data, &newblock_toolprocs);
}

static void use_move_block_tool(void* data)
{
    use_tool(data, &moveblock_toolprocs);
}

static void use_move_vertex_tool(void* data)
{
    use_tool(data, &movevertex_toolprocs);
}

static void use_xy_plane_constraint(void* data)
{
    ((mainwindow*)data)->constraint = CONSTRAIN_TO_XY_PLANE;
}

static void use_z_axis_constraint(void *data)
{
    ((mainwindow*)data)->constraint = CONSTRAIN_TO_Z_AXIS;
}

static void create_actions(mainwindow *win)
{
    win->quit_action = ui_create_action(win->window);
    ui_set_action_text    (win->quit_action, "Q&uit");
    ui_set_action_shortcut(win->quit_action, "Ctrl+Q");
    ui_set_actionproc     (win->quit_action, close, win);

    win->cut_action = ui_create_action(win->window);
    ui_set_action_text    (win->cut_action, "Cu&t");
    ui_set_action_shortcut(win->cut_action, "Ctrl+X");
    ui_set_actionproc     (win->cut_action, cut, win);

    win->copy_action = ui_create_action(win->window);
    ui_set_action_text    (win->copy_action, "&Copy");
    ui_set_action_shortcut(win->copy_action, "Ctrl+C");
    ui_set_actionproc     (win->copy_action, copy, win);

    win->paste_action = ui_create_action(win->window);
    ui_set_action_text    (win->paste_action, "&Paste");
    ui_set_action_shortcut(win->paste_action, "Ctrl+V");
    ui_set_actionproc     (win->paste_action, paste, win);

    win->delete_action = ui_create_action(win->window);
    ui_set_action_text    (win->delete_action, "Delete");
    ui_set_action_shortcut(win->delete_action, "Delete");
    ui_set_actionproc     (win->delete_action, del, win);

    win->group_action = ui_create_action(win->window);
    ui_set_action_text    (win->group_action, "&Group");
    ui_set_action_shortcut(win->group_action, "Ctrl+G");
    ui_set_actionproc     (win->group_action, group_action, win);

    win->ungroup_action = ui_create_action(win->window);
    ui_set_action_text    (win->ungroup_action, "&Ungroup");
    ui_set_action_shortcut(win->ungroup_action, "Ctrl+Alt+G");
    ui_set_actionproc     (win->ungroup_action, ungroup, win);

    win->new_block_action = ui_create_action(win->window);
    ui_set_action_text     (win->new_block_action, "New Block");
    ui_set_action_checkable(win->new_block_action, true);
    ui_set_actionproc      (win->new_block_action, use_new_block_tool, win);

    win->move_block_action = ui_create_action(win->window);
    ui_set_action_text     (win->move_block_action, "Move Block");
    ui_set_action_checkable(win->move_block_action, true);
    ui_set_actionproc      (win->move_block_action, use_move_block_tool, win);

    win->move_vertex_action_ = ui_create_action(win->window);
    ui_set_action_text     (win->move_vertex_action_, "Move Vertex");
    ui_set_action_checkable(win->move_vertex_action_, true);
    ui_set_actionproc      (win->move_vertex_action_, use_move_vertex_tool, win);

    ui_actiongroup *toolgroup = ui_create_actiongroup(win->window);
    ui_add_action_to_group(toolgroup, win->new_block_action);
    ui_add_action_to_group(toolgroup, win->move_block_action);
    ui_add_action_to_group(toolgroup, win->move_vertex_action_);
    ui_trig_action(win->new_block_action);

    win->xyplane_constraint_action = ui_create_action(win->window);
    ui_set_action_text     (win->xyplane_constraint_action, "XY");
    ui_set_action_checkable(win->xyplane_constraint_action, true);
    ui_set_action_shortcut (win->xyplane_constraint_action, "X");
    ui_set_actionproc      (win->xyplane_constraint_action, use_xy_plane_constraint, win);

    win->zaxis_constraint_action = ui_create_action(win->window);
    ui_set_action_text     (win->zaxis_constraint_action, "Z");
    ui_set_action_checkable(win->zaxis_constraint_action, true);
    ui_set_action_shortcut (win->zaxis_constraint_action, "Z");
    ui_set_actionproc      (win->zaxis_constraint_action, use_z_axis_constraint, win);

    ui_actiongroup *constraintgroup = ui_create_actiongroup(win->window);
    ui_add_action_to_group(constraintgroup, win->xyplane_constraint_action);
    ui_add_action_to_group(constraintgroup, win->zaxis_constraint_action);
    ui_trig_action(win->xyplane_constraint_action);
}

static void create_menus(mainwindow *win)
{
    win->filemenu = ui_create_menu(win->window);
    ui_set_menu_title  (win->filemenu, "&File");
    ui_add_menu_action (win->filemenu, win->quit_action);

    win->editmenu = ui_create_menu(win->window);
    ui_set_menu_title    (win->editmenu, "&Edit");
    ui_add_menu_action   (win->editmenu, win->cut_action);
    ui_add_menu_action   (win->editmenu, win->copy_action);
    ui_add_menu_action   (win->editmenu, win->paste_action);
    ui_add_menu_action   (win->editmenu, win->delete_action);
    ui_add_menu_separator(win->editmenu);
    ui_add_menu_action   (win->editmenu, win->group_action);
    ui_add_menu_action   (win->editmenu, win->ungroup_action);
}

static void create_toolbar(mainwindow *win)
{
    win->toolbar = ui_create_toolbar(win->window);
    ui_add_toolbar_action(win->toolbar, win->new_block_action);
    ui_add_toolbar_action(win->toolbar, win->move_block_action);
    ui_add_toolbar_action(win->toolbar, win->move_vertex_action_);
    ui_add_toolbar_separator(win->toolbar);
    ui_add_toolbar_action(win->toolbar, win->xyplane_constraint_action);
    ui_add_toolbar_action(win->toolbar, win->zaxis_constraint_action);
}

static void init_gl(void *data)
{
    glEnable(GL_CULL_FACE);
}

static void resize_gl(void *data, int width, int height)
{
    mainwindow *win = data;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    mat4 matrix = projectionmatrix(win);
    glLoadMatrixf(&matrix.x.x);
}

static void draw_gl(void *data)
{
    mainwindow *win = data;

    glMatrixMode(GL_MODELVIEW);
    mat4 matrix = modelviewmatrix(win);
    glLoadMatrixf(&matrix.x.x);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    draw(win->gridvertices);
    draw_group(get_root_group(win->document));

    if (win->toolprocs && win->toolprocs->draw_gl)
    {
        win->toolprocs->draw_gl(win->tooldata);
    }
}

static ui_glprocs glprocs =
{
    init_gl,
    resize_gl,
    draw_gl
};

static void key_pressed(void *data, ui_key key)
{
    mainwindow *win = data;
    movecamera_toolprocs.key_pressed(win->movecameratool, key);
}

static void key_released(void *data, ui_key key)
{
    mainwindow *win = data;
    movecamera_toolprocs.key_released(win->movecameratool, key);
}

static ui_keyprocs keyprocs =
{
    key_pressed,
    key_released
};

static void mouse_pressed(void *data, ui_mouseevent const *event)
{
    mainwindow *win = data;
    rotatecamera_toolprocs.mouse_pressed(win->rotatecameratool, event);

    if (win->toolprocs && win->toolprocs->mouse_pressed)
    {
        win->toolprocs->mouse_pressed(win->tooldata, event);
    }
}

static void mouse_released(void *data, ui_mouseevent const *event)
{
    mainwindow *win = data;
    rotatecamera_toolprocs.mouse_released(win->rotatecameratool, event);

    if (win->toolprocs && win->toolprocs->mouse_released)
    {
        win->toolprocs->mouse_released(win->tooldata, event);
    }
}

static void mouse_moved(void *data, ui_mouseevent const *event)
{
    mainwindow *win = data;
    rotatecamera_toolprocs.mouse_moved(win->rotatecameratool, event);

    if (win->toolprocs && win->toolprocs->mouse_moved)
    {
        win->toolprocs->mouse_moved(win->tooldata, event);
    }
}

ui_mouseprocs mouseprocs =
{
    mouse_pressed,
    mouse_released,
    mouse_moved,
};

mainwindow *create_mainwindow(void)
{
    mainwindow *win = calloc(1, sizeof(struct mainwindow));

    win->document = create_document();
    win->window = ui_create_window();
    win->glwidget = ui_create_glwidget(&glprocs, win);
    win->camera = create_camera();
    win->gridvertices = create_grid(10);
    win->cursorvertices = create_cursor(0.125f);
    win->constraint = CONSTRAIN_TO_XY_PLANE;

    vec3 camerapos = {0.0f, -4.0f, 2.0f};
    set_camera_position(win->camera, &camerapos);

    win->movecameratool = movecamera_toolprocs.create(win);
    win->rotatecameratool = rotatecamera_toolprocs.create(win);
    win->toolprocs = NULL;

    ui_set_keyprocs(win->glwidget, &keyprocs, win);
    ui_set_mouseprocs(win->glwidget, &mouseprocs, win);
    ui_resize_window(win->window, 800, 600);
    ui_set_center_widget(win->window, win->glwidget);
    ui_focus_widget(win->glwidget);

    create_actions(win);
    create_menus(win);
    create_toolbar(win);

    return win;
}

void destroy_mainwindow(mainwindow *win)
{
    movecamera_toolprocs.destroy(win->movecameratool);
    rotatecamera_toolprocs.destroy(win->rotatecameratool);
    free_tool(win);

    destroy_buffer(win->cursorvertices);
    destroy_buffer(win->gridvertices);
    destroy_camera(win->camera);
    ui_destroy_window(win->window);
    destroy_document(win->document);

    free(win);
}

void show_mainwindow(mainwindow *win)
{
    ui_show_window(win->window);
}

document *get_document(mainwindow *win)
{
    return win->document;
}

ui_glwidget *get_glwidget(mainwindow *win)
{
    return win->glwidget;
}

camera *get_camera(mainwindow *win)
{
    return win->camera;
}

static void doubles_to_floats(double *doubles, float const *floats, size_t n)
{
    for (size_t i = 0; i < n; ++i)
    {
        doubles[i] = floats[i];
    }
}

static ray make_pick_ray(float           mousex,
                         float           mousey,
                         mat4 const     *model,
                         mat4 const     *projection,
                         viewport const *viewport)
{
    double wx = mousex;
    double wy = viewport->height - mousey;
    double modeld[16], projectiond[16];
    double px1, py1, pz1, px2, py2, pz2;

    doubles_to_floats(modeld, &model->x.x, 16);
    doubles_to_floats(projectiond, &projection->x.x, 16);

    gluUnProject(wx, wy, 0.0, modeld, projectiond, &viewport->x, &px1, &py1, &pz1);
    gluUnProject(wx, wy, 1.0, modeld, projectiond, &viewport->x, &px2, &py2, &pz2);

    return (ray){{px1, py1, pz1}, {px2, py2, pz2}};
}

ray make_mouseray(mainwindow const *win, float mousex, float mousey)
{
    mat4 model = modelviewmatrix(win);
    mat4 projection = projectionmatrix(win);
    viewport viewport = {0, 0, ui_widget_width(win->glwidget), ui_widget_height(win->glwidget)};

    return make_pick_ray(mousex, mousey, &model, &projection, &viewport);
}

buffer *get_cursorvertices(mainwindow const *win)
{
    return win->cursorvertices;
}

constraint get_constrainalgorithm(mainwindow const *win)
{
    return win->constraint;
}

bool window_pick_vertex(mainwindow const *win, float mousex, float mousey, struct cornerref *cornerref)
{
    struct group *group      = get_root_group(win->document);
    mat4          model      = modelviewmatrix(win);
    mat4          projection = projectionmatrix(win);
    viewport      viewport   = {0, 0, ui_widget_width(win->glwidget), ui_widget_height(win->glwidget)};
    vec2          mouse      = {mousex, mousey};

    if (pick_vertex(group, &model, &projection, &viewport, &mouse, cornerref))
    {
        vec3 camerapos; get_camera_position(win->camera, &camerapos);
        vec3 vertexpos = cornerref_position(cornerref);
        ray cameraray; ray_from_point_to_point(&cameraray, &camerapos, &vertexpos);
        pick pick = pick_block(group, &cameraray);

        if (pick.blockref.block)
        {
            if (vec3_equals(&pick.triangle.a, &vertexpos) ||
                vec3_equals(&pick.triangle.b, &vertexpos) ||
                vec3_equals(&pick.triangle.c, &vertexpos))
            {
                return true;
            }

            if (vec3_distance(&camerapos, &vertexpos) >
                vec3_distance(&camerapos, &pick.intersection))
            {
                return false;
            }
        }
        else
        {
            return true;
        }
    }

    return false;
}

mat4 projectionmatrix(mainwindow const *win)
{
    mat4 axisadjust;
    mat4_rotationx(&axisadjust, -M_TAU_4);

    mat4 perspective;
    mat4_perspective(&perspective,
                    60.0f,
                    (float)ui_widget_width(win->glwidget) / (float)ui_widget_height(win->glwidget),
                    0.1f,
                    1000.0f);

    mat4 proj;
    mat4_transform(&proj, &axisadjust, &perspective);
    return proj;
}

mat4 modelviewmatrix(mainwindow const *win)
{
    mat4 transform, inverse;

    camera_worldtransform(win->camera, &transform);
    mat4_inverse(&inverse, &transform);
    return inverse;
}
