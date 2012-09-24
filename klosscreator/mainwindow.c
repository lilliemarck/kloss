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
    Document* document_;
    ui_window* window_;
    ui_glwidget* glwidget_;
    Camera *camera_;
    Buffer *gridvertices_;
    Buffer *cursorvertices_;
    ConstrainAlgorithm constrainalgorithm_;
    void *movecameratool_;
    void *rotatecameratool_;
    toolprocs const *toolprocs_;
    void *tooldata_;
    ui_action* quit_action_;
    ui_action* cut_action_;
    ui_action* copy_action_;
    ui_action* paste_action_;
    ui_action* del_action_;
    ui_action* group_action_;
    ui_action* ungroup_action_;
    ui_action* new_block_action_;
    ui_action* move_block_action_;
    ui_action* move_vertex_action_;
    ui_action* xy_plane_constraint_action_;
    ui_action* z_azis_constraint_action_;
    ui_menu* file_menu_;
    ui_menu* edit_menu_;
    ui_toolbar* tool_bar_;
};

static void close(void* data)
{
    mainwindow* self = data;
    ui_close_window(self->window_);
}

static void cut(void* data)
{
    mainwindow* self = data;
    CopySelectedBlocks(self->document_);
    DeleteSelectedBlocks(self->document_);
    ui_update_widget(self->glwidget_);
}

static void copy(void* data)
{
    mainwindow* self = data;
    CopySelectedBlocks(self->document_);
}

static void paste(void* data)
{
    mainwindow* self = data;
    PasteSelectedBlocks(self->document_);
    ui_update_widget(self->glwidget_);
}

static void del(void* data)
{
    mainwindow* self = data;
    DeleteSelectedBlocks(self->document_);
    ui_update_widget(self->glwidget_);
}

static void group(void* data)
{
    mainwindow* self = data;
    GroupSelectedBlocks(self->document_);
    ui_update_widget(self->glwidget_);
}

static void ungroup(void* data)
{
}

static void free_tool(mainwindow *win)
{
    if (win->toolprocs_)
    {
        win->toolprocs_->destroy(win->tooldata_);
        win->toolprocs_ = NULL;
    }
}

static void use_tool(mainwindow *win, toolprocs const *procs)
{
    free_tool(win);

    win->toolprocs_ = procs;
    win->tooldata_  = procs->create(win);

    ui_update_widget(win->glwidget_);
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
    ((mainwindow*)data)->constrainalgorithm_ = CONSTRAIN_TO_XY_PLANE;
}

static void use_z_axis_constraint(void *data)
{
    ((mainwindow*)data)->constrainalgorithm_ = CONSTRAIN_TO_Z_AXIS;
}

static void create_actions(mainwindow *win)
{
    win->quit_action_ = ui_create_action(win->window_);
    ui_set_action_text    (win->quit_action_, "Q&uit");
    ui_set_action_shortcut(win->quit_action_, "Ctrl+Q");
    ui_set_actionproc     (win->quit_action_, close, win);

    win->cut_action_ = ui_create_action(win->window_);
    ui_set_action_text    (win->cut_action_, "Cu&t");
    ui_set_action_shortcut(win->cut_action_, "Ctrl+X");
    ui_set_actionproc     (win->cut_action_, cut, win);

    win->copy_action_ = ui_create_action(win->window_);
    ui_set_action_text    (win->copy_action_, "&Copy");
    ui_set_action_shortcut(win->copy_action_, "Ctrl+C");
    ui_set_actionproc     (win->copy_action_, copy, win);

    win->paste_action_ = ui_create_action(win->window_);
    ui_set_action_text    (win->paste_action_, "&Paste");
    ui_set_action_shortcut(win->paste_action_, "Ctrl+V");
    ui_set_actionproc     (win->paste_action_, paste, win);

    win->del_action_ = ui_create_action(win->window_);
    ui_set_action_text    (win->del_action_, "Delete");
    ui_set_action_shortcut(win->del_action_, "Delete");
    ui_set_actionproc     (win->del_action_, del, win);

    win->group_action_ = ui_create_action(win->window_);
    ui_set_action_text    (win->group_action_, "&Group");
    ui_set_action_shortcut(win->group_action_, "Ctrl+G");
    ui_set_actionproc     (win->group_action_, group, win);

    win->ungroup_action_ = ui_create_action(win->window_);
    ui_set_action_text    (win->ungroup_action_, "&Ungroup");
    ui_set_action_shortcut(win->ungroup_action_, "Ctrl+Alt+G");
    ui_set_actionproc     (win->ungroup_action_, ungroup, win);

    win->new_block_action_ = ui_create_action(win->window_);
    ui_set_action_text     (win->new_block_action_, "New Block");
    ui_set_action_checkable(win->new_block_action_, true);
    ui_set_actionproc      (win->new_block_action_, use_new_block_tool, win);

    win->move_block_action_ = ui_create_action(win->window_);
    ui_set_action_text     (win->move_block_action_, "Move Block");
    ui_set_action_checkable(win->move_block_action_, true);
    ui_set_actionproc      (win->move_block_action_, use_move_block_tool, win);

    win->move_vertex_action_ = ui_create_action(win->window_);
    ui_set_action_text     (win->move_vertex_action_, "Move Vertex");
    ui_set_action_checkable(win->move_vertex_action_, true);
    ui_set_actionproc      (win->move_vertex_action_, use_move_vertex_tool, win);

    ui_actiongroup *toolgroup = ui_create_actiongroup(win->window_);
    ui_add_action_to_group(toolgroup, win->new_block_action_);
    ui_add_action_to_group(toolgroup, win->move_block_action_);
    ui_add_action_to_group(toolgroup, win->move_vertex_action_);
    ui_trig_action(win->new_block_action_);

    win->xy_plane_constraint_action_ = ui_create_action(win->window_);
    ui_set_action_text     (win->xy_plane_constraint_action_, "XY");
    ui_set_action_checkable(win->xy_plane_constraint_action_, true);
    ui_set_action_shortcut (win->xy_plane_constraint_action_, "X");
    ui_set_actionproc      (win->xy_plane_constraint_action_, use_xy_plane_constraint, win);

    win->z_azis_constraint_action_ = ui_create_action(win->window_);
    ui_set_action_text     (win->z_azis_constraint_action_, "Z");
    ui_set_action_checkable(win->z_azis_constraint_action_, true);
    ui_set_action_shortcut (win->z_azis_constraint_action_, "Z");
    ui_set_actionproc      (win->z_azis_constraint_action_, use_z_axis_constraint, win);

    ui_actiongroup *constraintgroup = ui_create_actiongroup(win->window_);
    ui_add_action_to_group(constraintgroup, win->xy_plane_constraint_action_);
    ui_add_action_to_group(constraintgroup, win->z_azis_constraint_action_);
    ui_trig_action(win->xy_plane_constraint_action_);
}

static void create_menus(mainwindow *win)
{
    win->file_menu_ = ui_create_menu(win->window_);
    ui_set_menu_title  (win->file_menu_, "&File");
    ui_add_menu_action (win->file_menu_, win->quit_action_);

    win->edit_menu_ = ui_create_menu(win->window_);
    ui_set_menu_title    (win->edit_menu_, "&Edit");
    ui_add_menu_action   (win->edit_menu_, win->cut_action_);
    ui_add_menu_action   (win->edit_menu_, win->copy_action_);
    ui_add_menu_action   (win->edit_menu_, win->paste_action_);
    ui_add_menu_action   (win->edit_menu_, win->del_action_);
    ui_add_menu_separator(win->edit_menu_);
    ui_add_menu_action   (win->edit_menu_, win->group_action_);
    ui_add_menu_action   (win->edit_menu_, win->ungroup_action_);
}

static void create_toolbar(mainwindow *win)
{
    win->tool_bar_ = ui_create_toolbar(win->window_);
    ui_add_toolbar_action(win->tool_bar_, win->new_block_action_);
    ui_add_toolbar_action(win->tool_bar_, win->move_block_action_);
    ui_add_toolbar_action(win->tool_bar_, win->move_vertex_action_);
    ui_add_toolbar_separator(win->tool_bar_);
    ui_add_toolbar_action(win->tool_bar_, win->xy_plane_constraint_action_);
    ui_add_toolbar_action(win->tool_bar_, win->z_azis_constraint_action_);
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
    Mat4 matrix = projectionmatrix(win);
    glLoadMatrixf(&matrix.X.X);
}

static void draw_gl(void *data)
{
    mainwindow *win = data;

    glMatrixMode(GL_MODELVIEW);
    Mat4 matrix = modelviewmatrix(win);
    glLoadMatrixf(&matrix.X.X);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    draw(win->gridvertices_);
    DrawGroup(GetRootGroup(win->document_));

    if (win->toolprocs_ && win->toolprocs_->draw_gl)
    {
        win->toolprocs_->draw_gl(win->tooldata_);
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
    movecamera_toolprocs.key_pressed(win->movecameratool_, key);
}

static void key_released(void *data, ui_key key)
{
    mainwindow *win = data;
    movecamera_toolprocs.key_released(win->movecameratool_, key);
}

static ui_keyprocs keyprocs =
{
    key_pressed,
    key_released
};

static void mouse_pressed(void *data, ui_mouseevent const *event)
{
    mainwindow *win = data;
    rotatecamera_toolprocs.mouse_pressed(win->rotatecameratool_, event);

    if (win->toolprocs_ && win->toolprocs_->mouse_pressed)
    {
        win->toolprocs_->mouse_pressed(win->tooldata_, event);
    }
}

static void mouse_released(void *data, ui_mouseevent const *event)
{
    mainwindow *win = data;
    rotatecamera_toolprocs.mouse_released(win->rotatecameratool_, event);

    if (win->toolprocs_ && win->toolprocs_->mouse_released)
    {
        win->toolprocs_->mouse_released(win->tooldata_, event);
    }
}

static void mouse_moved(void *data, ui_mouseevent const *event)
{
    mainwindow *win = data;
    rotatecamera_toolprocs.mouse_moved(win->rotatecameratool_, event);

    if (win->toolprocs_ && win->toolprocs_->mouse_moved)
    {
        win->toolprocs_->mouse_moved(win->tooldata_, event);
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
    mainwindow *win = calloc(1, sizeof(mainwindow));

    win->document_ = CreateDocument();
    win->window_ = ui_create_window();
    win->glwidget_ = ui_create_glwidget(&glprocs, win);
    win->camera_ = CreateCamera();
    win->gridvertices_ = create_grid(10);
    win->cursorvertices_ = create_cursor(0.125f);
    win->constrainalgorithm_ = CONSTRAIN_TO_XY_PLANE;

    Vec3 camerapos = {0.0f, -4.0f, 2.0f};
    SetCameraPosition(win->camera_, &camerapos);

    win->movecameratool_ = movecamera_toolprocs.create(win);
    win->rotatecameratool_ = rotatecamera_toolprocs.create(win);
    win->toolprocs_ = NULL;

    ui_set_keyprocs(win->glwidget_, &keyprocs, win);
    ui_set_mouseprocs(win->glwidget_, &mouseprocs, win);
    ui_resize_window(win->window_, 800, 600);
    ui_set_center_widget(win->window_, win->glwidget_);
    ui_focus_widget(win->glwidget_);

    create_actions(win);
    create_menus(win);
    create_toolbar(win);

    return win;
}

void destroy_mainwindow(mainwindow *win)
{
    movecamera_toolprocs.destroy(win->movecameratool_);
    rotatecamera_toolprocs.destroy(win->rotatecameratool_);
    free_tool(win);

    DestroyBuffer(win->cursorvertices_);
    DestroyBuffer(win->gridvertices_);
    DestroyCamera(win->camera_);
    ui_destroy_window(win->window_);
    DestroyDocument(win->document_);

    free(win);
}

void show_mainwindow(mainwindow *win)
{
    ui_show_window(win->window_);
}

Document *get_document(mainwindow *win)
{
    return win->document_;
}

ui_glwidget *get_glwidget(mainwindow *win)
{
    return win->glwidget_;
}

Camera *get_camera(mainwindow *win)
{
    return win->camera_;
}

static void doubles_to_floats(double *doubles, float const *floats, size_t n)
{
    for (size_t i = 0; i < n; ++i)
    {
        doubles[i] = floats[i];
    }
}

static Ray make_pick_ray(float           mousex,
                         float           mousey,
                         Mat4 const     *model,
                         Mat4 const     *projection,
                         Viewport const *viewport)
{
    double wx = mousex;
    double wy = viewport->height - mousey;
    double modeld[16], projectiond[16];
    double px1, py1, pz1, px2, py2, pz2;

    doubles_to_floats(modeld, &model->X.X, 16);
    doubles_to_floats(projectiond, &projection->X.X, 16);

    gluUnProject(wx, wy, 0.0, modeld, projectiond, &viewport->x, &px1, &py1, &pz1);
    gluUnProject(wx, wy, 1.0, modeld, projectiond, &viewport->x, &px2, &py2, &pz2);

    return (Ray){{px1, py1, pz1}, {px2, py2, pz2}};
}

Ray make_mouseray(mainwindow const *win, float mousex, float mousey)
{
    Mat4 model = modelviewmatrix(win);
    Mat4 projection = projectionmatrix(win);
    Viewport viewport = {0, 0, ui_widget_width(win->glwidget_), ui_widget_height(win->glwidget_)};

    return make_pick_ray(mousex, mousey, &model, &projection, &viewport);
}

Buffer *get_cursorvertices(mainwindow const *win)
{
    return win->cursorvertices_;
}

ConstrainAlgorithm get_constrainalgorithm(mainwindow const *win)
{
    return win->constrainalgorithm_;
}

bool pick_vertex(mainwindow const *win, float mousex, float mousey, struct CornerRef *cornerref)
{
    Group*   group      = GetRootGroup(win->document_);
    Mat4     model      = modelviewmatrix(win);
    Mat4     projection = projectionmatrix(win);
    Viewport viewport   = {0, 0, ui_widget_width(win->glwidget_), ui_widget_height(win->glwidget_)};
    Vec2     mouse      = {mousex, mousey};

    if (PickGroupVertex(group, &model, &projection, &viewport, &mouse, cornerref))
    {
        Vec3 camerapos; GetCameraPosition(win->camera_, &camerapos);
        Vec3 vertexpos = CornerRefPosition(cornerref);
        Ray cameraray; RayFromPointToPoint(&cameraray, &camerapos, &vertexpos);
        Pick pick = PickGroupBlock(group, &cameraray);

        if (pick.block)
        {
            if (Vec3Equals(&pick.triangle.A, &vertexpos) ||
                Vec3Equals(&pick.triangle.B, &vertexpos) ||
                Vec3Equals(&pick.triangle.C, &vertexpos))
            {
                return true;
            }

            if (Vec3Distance(&camerapos, &vertexpos) >
                Vec3Distance(&camerapos, &pick.intersection))
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

Mat4 projectionmatrix(mainwindow const *win)
{
    Mat4 axisadjust;
    Mat4RotationX(&axisadjust, -M_TAU_4);

    Mat4 perspective;
    Mat4Perspective(&perspective,
                    60.0f,
                    (float)ui_widget_width(win->glwidget_) / (float)ui_widget_height(win->glwidget_),
                    0.1f,
                    1000.0f);

    Mat4 proj;
    Mat4Transform(&proj, &axisadjust, &perspective);
    return proj;
}

Mat4 modelviewmatrix(mainwindow const *win)
{
    Mat4 transform, inverse;

    CameraWorldTransform(win->camera_, &transform);
    Mat4Inverse(&inverse, &transform);
    return inverse;
}
