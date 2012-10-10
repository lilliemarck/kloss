#include "main_window.hpp"
#include <GL/glu.h>
#include <kloss/group.hpp>
#include <kloss/memory.hpp>
#include <klosscreator/group_algorithm.hpp>
#include <klosscreator/move_block_tool.hpp>
#include <klosscreator/move_vertex_tool.hpp>
#include <klosscreator/new_block_tool.hpp>
#include <klosscreator/tool.hpp>

ui_glprocs main_window::glprocs_ =
{
    init_gl,
    resize_gl,
    draw_gl
};

ui_keyprocs main_window::keyprocs_ =
{
    key_pressed,
    key_released
};

ui_mouseprocs main_window::mouseprocs_ =
{
    mouse_pressed,
    mouse_released,
    mouse_moved,
};

main_window::main_window()
    : document_(make_unique<document>())
    , window_(ui_create_window())
    , gl_widget_(ui_create_glwidget(&glprocs_, this))
    , grid_vertices_(create_grid(10))
    , cursor_vertices_(create_cursor(0.125f))
    , constrain_mode_(contrain_to_xy_plane)
    , move_camera_tool_(*this)
    , rotate_camera_tool_(*this)
{
    camera_.set_position({0.0f, -4.0f, 2.0f});

    ui_set_keyprocs(gl_widget_, &keyprocs_, this);
    ui_set_mouseprocs(gl_widget_, &mouseprocs_, this);

    ui_resize_window(window_, 800, 600);
    ui_set_center_widget(window_, gl_widget_);
    ui_focus_widget(gl_widget_);

    create_actions();
    create_menus();
    create_toolbar();
}

main_window::~main_window()
{
    ui_destroy_window(window_);
}

void main_window::show()
{
    ui_show_window(window_);
}

document& main_window::get_document()
{
    return *document_;
}

ui_glwidget* main_window::get_gl_widget()
{
    return gl_widget_;
}

camera& main_window::get_camera()
{
    return camera_;
}

vertex_array const& main_window::get_cursor_vertices() const
{
    return cursor_vertices_;
}

constrain_mode main_window::get_constrain_mode() const
{
    return constrain_mode_;
}

static void doubles_to_floats(double* doubles, float const* floats, std::size_t n)
{
    for (std::size_t i = 0; i < n; ++i)
    {
        doubles[i] = floats[i];
    }
}

static ray make_pick_ray(float mousex, float mousey, mat4 const& model, mat4 const& projection, viewport const& viewport)
{
    double wx = mousex;
    double wy = viewport.height - mousey;
    double modeld[16], projectiond[16];
    double px1, py1, pz1, px2, py2, pz2;

    doubles_to_floats(modeld, &model.x.x, 16);
    doubles_to_floats(projectiond, &projection.x.x, 16);

    gluUnProject(wx, wy, 0.0, modeld, projectiond, &viewport.x, &px1, &py1, &pz1);
    gluUnProject(wx, wy, 1.0, modeld, projectiond, &viewport.x, &px2, &py2, &pz2);

    return {{float(px1), float(py1), float(pz1)},
            {float(px2), float(py2), float(pz2)}};
}

ray main_window::make_mouse_ray(float mouse_x, float mouse_y) const
{
    mat4 modelview = modelview_matrix();
    mat4 projection = projection_matrix();
    viewport viewport = {0, 0, ui_widget_width(gl_widget_), ui_widget_height(gl_widget_)};

    return make_pick_ray(mouse_x, mouse_y, modelview, projection, viewport);
}

boost::optional<corner_ref> main_window::pick_vertex(float mouse_x, float mouse_y) const
{
    group_ptr group      = document_->get_root_group();
    mat4      model      = modelview_matrix();
    mat4      projection = projection_matrix();
    viewport  viewport   = {0, 0, ui_widget_width(gl_widget_), ui_widget_height(gl_widget_)};
    vec2      mouse      = {mouse_x, mouse_y};

    if (auto ref = pick_corner(group, model, projection, viewport, mouse))
    {
        vec3 camerapos = camera_.get_position();
        vec3 vertexpos = get_position(*ref);
        ray cameraray = make_ray_to(camerapos, vertexpos);
        pick pick = pick_block(group, cameraray);

        if (pick.block_ref.block)
        {
            if ((pick.triangle.a == vertexpos) ||
                (pick.triangle.b == vertexpos) ||
                (pick.triangle.c == vertexpos))
            {
                return ref;
            }

            if (distance(camerapos, vertexpos) >
                distance(camerapos, pick.intersection))
            {
                return {};
            }
        }
        else
        {
            return ref;
        }
    }

    return {};
}

mat4 main_window::projection_matrix() const
{
    mat4 axis_adjust = rotation_matrix_x(-tau_over_4);

    mat4 perspective = perspective_matrix(
        60.0f,
        float(ui_widget_width(gl_widget_)) / float(ui_widget_height(gl_widget_)),
        0.1f,
        1000.0f);

    return perspective * axis_adjust;
}

mat4 main_window::modelview_matrix() const
{
    return inverse(world_transform(camera_));
}

void main_window::create_actions()
{
    quit_action_ = ui_create_action(window_);
    ui_set_action_text    (quit_action_, "Q&uit");
    ui_set_action_shortcut(quit_action_, "Ctrl+Q");
    ui_set_actionproc     (quit_action_, close, this);

    cut_action_ = ui_create_action(window_);
    ui_set_action_text    (cut_action_, "Cu&t");
    ui_set_action_shortcut(cut_action_, "Ctrl+X");
    ui_set_actionproc     (cut_action_, cut, this);

    copy_action_ = ui_create_action(window_);
    ui_set_action_text    (copy_action_, "&Copy");
    ui_set_action_shortcut(copy_action_, "Ctrl+C");
    ui_set_actionproc     (copy_action_, copy, this);

    paste_action_ = ui_create_action(window_);
    ui_set_action_text    (paste_action_, "&Paste");
    ui_set_action_shortcut(paste_action_, "Ctrl+V");
    ui_set_actionproc     (paste_action_, paste, this);

    delete_action_ = ui_create_action(window_);
    ui_set_action_text    (delete_action_, "Delete");
    ui_set_action_shortcut(delete_action_, "Delete");
    ui_set_actionproc     (delete_action_, del, this);

    group_action_ = ui_create_action(window_);
    ui_set_action_text    (group_action_, "&Group");
    ui_set_action_shortcut(group_action_, "Ctrl+G");
    ui_set_actionproc     (group_action_, group, this);

    ungroup_action_ = ui_create_action(window_);
    ui_set_action_text    (ungroup_action_, "&Ungroup");
    ui_set_action_shortcut(ungroup_action_, "Ctrl+Alt+G");
    ui_set_actionproc     (ungroup_action_, ungroup, this);

    new_block_action_ = ui_create_action(window_);
    ui_set_action_text     (new_block_action_, "New Block");
    ui_set_action_checkable(new_block_action_, true);
    ui_set_actionproc      (new_block_action_, use_new_block_tool, this);

    move_block_action_ = ui_create_action(window_);
    ui_set_action_text     (move_block_action_, "Move Block");
    ui_set_action_checkable(move_block_action_, true);
    ui_set_actionproc      (move_block_action_, use_move_block_tool, this);

    move_vertex_action_ = ui_create_action(window_);
    ui_set_action_text     (move_vertex_action_, "Move Vertex");
    ui_set_action_checkable(move_vertex_action_, true);
    ui_set_actionproc      (move_vertex_action_, use_move_vertex_tool, this);

    ui_actiongroup* toolgroup = ui_create_actiongroup(window_);
    ui_add_action_to_group(toolgroup, new_block_action_);
    ui_add_action_to_group(toolgroup, move_block_action_);
    ui_add_action_to_group(toolgroup, move_vertex_action_);
    ui_trig_action(new_block_action_);

    xy_plane_constraint_action_ = ui_create_action(window_);
    ui_set_action_text     (xy_plane_constraint_action_, "XY");
    ui_set_action_checkable(xy_plane_constraint_action_, true);
    ui_set_action_shortcut (xy_plane_constraint_action_, "X");
    ui_set_actionproc      (xy_plane_constraint_action_, use_xy_plane_constraint, this);

    z_axis_constraint_action_ = ui_create_action(window_);
    ui_set_action_text     (z_axis_constraint_action_, "Z");
    ui_set_action_checkable(z_axis_constraint_action_, true);
    ui_set_action_shortcut (z_axis_constraint_action_, "Z");
    ui_set_actionproc      (z_axis_constraint_action_, use_z_axis_constraint, this);

    ui_actiongroup* constraintgroup = ui_create_actiongroup(window_);
    ui_add_action_to_group(constraintgroup, xy_plane_constraint_action_);
    ui_add_action_to_group(constraintgroup, z_axis_constraint_action_);
    ui_trig_action(xy_plane_constraint_action_);
}

void main_window::create_menus()
{
    file_menu_ = ui_create_menu(window_);
    ui_set_menu_title  (file_menu_, "&File");
    ui_add_menu_action (file_menu_, quit_action_);

    edit_menu_ = ui_create_menu(window_);
    ui_set_menu_title    (edit_menu_, "&Edit");
    ui_add_menu_action   (edit_menu_, cut_action_);
    ui_add_menu_action   (edit_menu_, copy_action_);
    ui_add_menu_action   (edit_menu_, paste_action_);
    ui_add_menu_action   (edit_menu_, delete_action_);
    ui_add_menu_separator(edit_menu_);
    ui_add_menu_action   (edit_menu_, group_action_);
    ui_add_menu_action   (edit_menu_, ungroup_action_);
}

void main_window::create_toolbar()
{
    toolbar_ = ui_create_toolbar(window_);
    ui_add_toolbar_action(toolbar_, new_block_action_);
    ui_add_toolbar_action(toolbar_, move_block_action_);
    ui_add_toolbar_action(toolbar_, move_vertex_action_);
    ui_add_toolbar_separator(toolbar_);
    ui_add_toolbar_action(toolbar_, xy_plane_constraint_action_);
    ui_add_toolbar_action(toolbar_, z_axis_constraint_action_);
}

void main_window::use_tool(std::unique_ptr<tool>&& tool)
{
    tool_ = std::move(tool);
    ui_update_widget(gl_widget_);
}

void main_window::close(void* data)
{
    main_window* self = static_cast<main_window*>(data);
    ui_close_window(self->window_);
}

void main_window::cut(void* data)
{
    main_window* self = static_cast<main_window*>(data);
    self->document_->copy();
    self->document_->del();
    ui_update_widget(self->gl_widget_);
}

void main_window::copy(void* data)
{
    main_window* self = static_cast<main_window*>(data);
    self->document_->copy();
}

void main_window::paste(void* data)
{
    main_window* self = static_cast<main_window*>(data);
    self->document_->paste();
    ui_update_widget(self->gl_widget_);
}

void main_window::del(void* data)
{
    main_window* self = static_cast<main_window*>(data);
    self->document_->del();
    ui_update_widget(self->gl_widget_);
}

void main_window::group(void* data)
{
    main_window* self = static_cast<main_window*>(data);
    self->document_->group();
    ui_update_widget(self->gl_widget_);
}

void main_window::ungroup(void* data)
{
    main_window* self = static_cast<main_window*>(data);
    self->document_->ungroup();
    ui_update_widget(self->gl_widget_);
}

void main_window::use_new_block_tool(void* data)
{
    main_window* self = static_cast<main_window*>(data);
    self->use_tool(make_unique<new_block_tool>(*self));
}

void main_window::use_move_block_tool(void* data)
{
    main_window* self = static_cast<main_window*>(data);
    self->use_tool(make_unique<move_block_tool>(*self));
}

void main_window::use_move_vertex_tool(void* data)
{
    main_window* self = static_cast<main_window*>(data);
    self->use_tool(make_unique<move_vertex_tool>(*self));
}

void main_window::use_xy_plane_constraint(void* data)
{
    main_window* self = static_cast<main_window*>(data);
    self->constrain_mode_ = contrain_to_xy_plane;
}

void main_window::use_z_axis_constraint(void* data)
{
    main_window* self = static_cast<main_window*>(data);
    self->constrain_mode_ = constrain_to_z_axis;
}

void main_window::init_gl(void* data)
{
    glEnable(GL_CULL_FACE);
}

void main_window::resize_gl(void* data, int width, int height)
{
    main_window* self = static_cast<main_window*>(data);

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    mat4 matrix = self->projection_matrix();
    glLoadMatrixf(&matrix.x.x);
}

void main_window::draw_gl(void* data)
{
    main_window* self = static_cast<main_window*>(data);

    glMatrixMode(GL_MODELVIEW);
    mat4 matrix = self->modelview_matrix();
    glLoadMatrixf(&matrix.x.x);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    draw(self->grid_vertices_);
    self->document_->get_root_group()->draw();

    if (self->tool_)
    {
        self->tool_->draw();
    }
}

void main_window::key_pressed(void* data, ui_key key)
{
    main_window* win = static_cast<main_window*>(data);
    win->move_camera_tool_.key_pressed(key);
}

void main_window::key_released(void* data, ui_key key)
{
    main_window* win = static_cast<main_window*>(data);
    win->move_camera_tool_.key_released(key);
}

void main_window::mouse_pressed(void* data, ui_mouseevent const* event)
{
    main_window* win = static_cast<main_window*>(data);
    win->rotate_camera_tool_.mouse_pressed(*event);

    if (win->tool_)
    {
        win->tool_->mouse_pressed(*event);
    }
}

void main_window::mouse_released(void* data, ui_mouseevent const* event)
{
    main_window* win = static_cast<main_window*>(data);
    win->rotate_camera_tool_.mouse_released(*event);

    if (win->tool_)
    {
        win->tool_->mouse_released(*event);
    }
}

void main_window::mouse_moved(void* data, ui_mouseevent const* event)
{
    main_window* win = static_cast<main_window*>(data);
    win->rotate_camera_tool_.mouse_moved(*event);

    if (win->tool_)
    {
        win->tool_->mouse_moved(*event);
    }
}
