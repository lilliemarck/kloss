#include "gl_widget.hpp"
#include <QMouseEvent>
#include <kloss/geometry.hpp>
#include <kloss/memory.hpp>
#include <klosscreator/document.hpp>
#include <klosscreator/move_block_tool.hpp>
#include <klosscreator/move_vertex_tool.hpp>
#include <klosscreator/new_block_tool.hpp>

#include <kloss/math.hpp>

namespace kloss {
namespace creator {

gl_widget::gl_widget(struct document& document)
    : QGLWidget(static_cast<QWidget*>(nullptr))
    , document_(document)
    , grid_(make_grid(10))
    , cursor_(make_cursor(0.125f))
    , constrain_algorithm_(constrain_algorithm::xy_plane)
    , move_camera_tool_(*this)
    , turn_camera_tool_(*this)
{
    setMouseTracking(true);
    camera_.set_position({0.0f, -4.0f, 2.0f});
}

gl_widget::~gl_widget()
{
}

document& gl_widget::document()
{
    return document_;
}

world& gl_widget::world()
{
    return document_.world;
}

camera& gl_widget::camera()
{
    return camera_;
}

ray gl_widget::mouse_ray(float mouse_x, float mouse_y) const
{
    cml::matrix44f_c viewport;
    matrix_viewport(viewport, 0.0f, float(width()), float(height()), 0.0f, cml::z_clip_zero);

    ray ray;
    make_pick_ray(mouse_x,
                  mouse_y,
                  modelview_matrix(),
                  projection_matrix(),
                  viewport,
                  ray.origin,
                  ray.direction,
                  false);

    return ray;
}

vertex_array const& gl_widget::cursor_vertices() const
{
    return cursor_;
}

constrain_algorithm gl_widget::get_constrain_algorithm() const
{
    return constrain_algorithm_;
}

boost::optional<corner_ref> gl_widget::pick_vertex(float mouse_x, float mouse_y) const
{
    viewport viewport = {0, 0, width(), height()};

    auto corner_ref = document_.world.pick_vertex(modelview_matrix(), projection_matrix(), viewport, {mouse_x, mouse_y});

    if (corner_ref)
    {
        auto vertex_position = to_vector(*corner_ref);
        auto pick = document_.world.pick_block(make_ray_to(camera_.get_position(), vertex_position));

        if (pick.block)
        {
            for (std::size_t i = 0; i < 3; ++i)
            {
                if (pick.triangle[i] == vertex_position)
                {
                    return corner_ref;
                }
            }

            if (distance(camera_.get_position(), vertex_position) >
                distance(camera_.get_position(), pick.intersection))
            {
                return {};
            }
        }
    }

    return corner_ref;
}

void gl_widget::use_new_block_tool()
{
    tool_ = make_unique<new_block_tool>(*this);
}

void gl_widget::use_move_block_tool()
{
    tool_ = make_unique<move_block_tool>(*this);
}

void gl_widget::use_move_vertex_tool()
{
    tool_ = make_unique<move_vertex_tool>(*this);
}

void gl_widget::use_xy_plane_constraint()
{
    constrain_algorithm_ = constrain_algorithm::xy_plane;
}

void gl_widget::use_z_axis_constraint()
{
    constrain_algorithm_ = constrain_algorithm::z_axis;
}

void gl_widget::initializeGL()
{
    glEnable(GL_CULL_FACE);
}

void gl_widget::keyPressEvent(QKeyEvent* event)
{
    move_camera_tool_.key_press_event(*event);
}

void gl_widget::keyReleaseEvent(QKeyEvent* event)
{
    move_camera_tool_.key_release_event(*event);
}

void gl_widget::mousePressEvent(QMouseEvent* event)
{
    turn_camera_tool_.mouse_press_event(*event);

    if (tool_)
    {
        tool_->mouse_press_event(*event);
    }
}

void gl_widget::mouseReleaseEvent(QMouseEvent* event)
{
    turn_camera_tool_.mouse_release_event(*event);

    if (tool_)
    {
        tool_->mouse_release_event(*event);
    }
}

void gl_widget::mouseMoveEvent(QMouseEvent* event)
{
    turn_camera_tool_.mouse_move_event(*event);

    if (tool_)
    {
        tool_->mouse_move_event(*event);
    }
}

void gl_widget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    cml::matrix44f_c matrix = projection_matrix();
    glLoadMatrixf(matrix.data());
}

void gl_widget::paintGL()
{
    glMatrixMode(GL_MODELVIEW);
    auto matrix = modelview_matrix();
    glLoadMatrixf(matrix.data());

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    draw(grid_);
    document_.world.draw();

    if (tool_)
    {
        tool_->paint_gl();
    }
}

cml::matrix44f_c gl_widget::projection_matrix() const
{
    cml::matrix44f_c axis_adjust;
    matrix_rotation_world_x(axis_adjust, -cml::constantsf::pi_over_2());

    cml::matrix44f_c matrix;
    matrix_perspective_yfov_RH(matrix, cml::rad(60.0f), float(width()) / float(height()), 0.1f, 1000.0f, cml::z_clip_neg_one);

    return matrix * axis_adjust;
}

cml::matrix44f_c gl_widget::modelview_matrix() const
{
    return inverse(world_transform(camera_));
}

float minor_size(QWidget const& widget)
{
    return std::min(widget.width(), widget.height());
}

} // namespace kloss
} // namespace creator
