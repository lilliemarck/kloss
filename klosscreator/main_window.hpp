#pragma once

#include <kloss/camera.hpp>
#include <klosscreator/constrain.hpp>
#include <klosscreator/document.hpp>
#include <klosscreator/move_camera_tool.hpp>
#include <klosscreator/rotate_camera_tool.hpp>
#include <klosscreator/tool.hpp>
#include <klosscreator/vertex_array.hpp>
#include <ui/ui.h>

class main_window
{
public:
    main_window();
    ~main_window();

    void show();
    document& get_document();
    ui_glwidget* get_gl_widget();
    camera& get_camera();
    vertex_array const& get_cursor_vertices() const;
    constrain_mode get_constrain_mode() const;

    ray make_mouse_ray(float mouse_x, float mouse_y) const;
    boost::optional<corner_ref> pick_vertex(float mouse_x, float mouse_y) const;

    mat4 projection_matrix() const;
    mat4 modelview_matrix() const;

private:
    void create_actions();
    void create_menus();
    void create_toolbar();
    void use_tool(std::unique_ptr<tool>&& tool);

    static void close(void* data);
    static void cut(void* data);
    static void copy(void* data);
    static void paste(void* data);
    static void del(void* data);
    static void group(void* data);
    static void ungroup(void* data);
    static void use_new_block_tool(void* data);
    static void use_move_block_tool(void* data);
    static void use_move_vertex_tool(void* data);
    static void use_xy_plane_constraint(void* data);
    static void use_z_axis_constraint(void* data);

    static void init_gl(void* data);
    static void resize_gl(void* data, int width, int height);
    static void draw_gl(void* data);
    static void key_pressed(void* data, ui_key key);
    static void key_released(void* data, ui_key key);
    static void mouse_pressed(void* data, ui_mouseevent const* event);
    static void mouse_released(void* data, ui_mouseevent const* event);
    static void mouse_moved(void* data, ui_mouseevent const* event);

    std::unique_ptr<document> document_;
    ui_window* window_;
    ui_glwidget* gl_widget_;
    camera camera_;
    vertex_array grid_vertices_;
    vertex_array cursor_vertices_;
    constrain_mode constrain_mode_;
    move_camera_tool move_camera_tool_;
    rotate_camera_tool rotate_camera_tool_;
    std::unique_ptr<tool> tool_;
    ui_action* quit_action_;
    ui_action* cut_action_;
    ui_action* copy_action_;
    ui_action* paste_action_;
    ui_action* delete_action_;
    ui_action* group_action_;
    ui_action* ungroup_action_;
    ui_action* new_block_action_;
    ui_action* move_block_action_;
    ui_action* move_vertex_action_;
    ui_action* xy_plane_constraint_action_;
    ui_action* z_axis_constraint_action_;
    ui_menu* file_menu_;
    ui_menu* edit_menu_;
    ui_toolbar* toolbar_;

    static ui_glprocs glprocs_;
    static ui_keyprocs keyprocs_;
    static ui_mouseprocs mouseprocs_;
};
