#pragma once

#include <memory>
#include <boost/optional.hpp>
#include <kloss/memory.hpp>
#include <kloss/group.h>
#include <klosscreator/constrain.h>
#include <klosscreator/move_camera_tool.hpp>
#include <klosscreator/turn_camera_tool.hpp>
#include <klosscreator/vertex_array.hpp>
#include <ui/ui.h>

struct Camera;
struct Document;

namespace kloss {
namespace creator {

class tool;

class gl_widget
{
public:
    gl_widget(Document* document);
    ~gl_widget();

    ui_glwidget *glwidget();
    Document* document();
    Group* group();
    Camera* camera();
    Ray mouse_ray(float mouse_x, float mouse_y) const;
    vertex_array const& cursor_vertices() const;
    ConstrainAlgorithm get_constrain_algorithm() const;
    boost::optional<CornerRef> pick_vertex(float mouse_x, float mouse_y) const;

    void use_new_block_tool();
    void use_move_block_tool();
    void use_move_vertex_tool();
    void use_xy_plane_constraint();
    void use_z_axis_constraint();

private:
    static void key_pressed(void *data, ui_key key);
    static void key_released(void *data, ui_key key);
    static void mouse_pressed(void *data, ui_mouseevent const* event);
    static void mouse_released(void *data, ui_mouseevent const* event);
    static void mouse_moved(void *data, ui_mouseevent const* event);
    static void init_gl(void *data);
    static void resize_gl(void *data, int width, int height);
    static void draw_gl(void *data);
    Mat4 projection_matrix() const;
    Mat4 modelview_matrix() const;

    template <typename Tool>
    void use_tool()
    {
        tool_ = make_unique<Tool>(*this);
        ui_update_widget(glwidget_);
    }

    ui_glwidget *glwidget_;
    Document* document_;
    std::shared_ptr<Camera> camera_;
    vertex_array grid_;
    vertex_array cursor_;
    ConstrainAlgorithm constrain_algorithm_;
    move_camera_tool move_camera_tool_;
    turn_camera_tool turn_camera_tool_;
    std::unique_ptr<tool> tool_;

    static ui_glprocs glprocs_;
    static ui_keyprocs keyprocs_;
    static ui_mouseprocs mouseprocs_;
};

} // namespace creator
} // namespace kloss
