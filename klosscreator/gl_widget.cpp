#include "gl_widget.hpp"
#include <QMouseEvent>
#include <kloss/camera.h>
#include <kloss/geometry.h>
#include <klosscreator/document.h>
#include <klosscreator/move_block_tool.hpp>
#include <klosscreator/move_vertex_tool.hpp>
#include <klosscreator/new_block_tool.hpp>

#include <kloss/math.hpp>
#include <GL/glu.h>
#include <math.h>

namespace kloss {
namespace creator {

gl_widget::gl_widget(Document* document)
    : QGLWidget(static_cast<QWidget*>(nullptr))
    , document_(document)
    , camera_(CreateCamera(), DestroyCamera)
    , grid_(make_grid(10))
    , cursor_(make_cursor(0.125f))
    , constrain_algorithm_(ConstrainAlgorithm::CONSTRAIN_TO_XY_PLANE)
    , move_camera_tool_(*this)
    , turn_camera_tool_(*this)
{
    setMouseTracking(true);

    Vec3 camera_pos = {0.0f, -4.0f, 2.0f};
    SetCameraPosition(camera_.get(), &camera_pos);
}

gl_widget::~gl_widget()
{
}

Document* gl_widget::document()
{
    return document_;
}

Group* gl_widget::group()
{
    return GetRootGroup(document_);
}

Camera* gl_widget::camera()
{
    return camera_.get();
}

static void doubles_to_floats(double *doubles, float const *floats, size_t n)
{
    for (size_t i = 0; i < n; ++i)
    {
        doubles[i] = floats[i];
    }
}

static Ray make_pick_ray(float mouse_x,
                         float mouse_y,
                         Mat4 const& model,
                         Mat4 const& projection,
                         Viewport const& viewport)
{
    double wx = mouse_x;
    double wy = viewport.height - mouse_y;
    double modeld[16], projectiond[16];
    double px1, py1, pz1, px2, py2, pz2;

    doubles_to_floats(modeld, &model.X.X, 16);
    doubles_to_floats(projectiond, &projection.X.X, 16);

    gluUnProject(wx, wy, 0.0, modeld, projectiond, &viewport.x, &px1, &py1, &pz1);
    gluUnProject(wx, wy, 1.0, modeld, projectiond, &viewport.x, &px2, &py2, &pz2);

    return {{float(px1), float(py1), float(pz1)}, {float(px2), float(py2), float(pz2)}};
}

Ray gl_widget::mouse_ray(float mouse_x, float mouse_y) const
{
    Mat4 model = modelview_matrix();
    Mat4 projection = projection_matrix();
    Viewport viewport = {0, 0, width(), height()};

    return make_pick_ray(mouse_x, mouse_y, model, projection, viewport);
}

vertex_array const& gl_widget::cursor_vertices() const
{
    return cursor_;
}

ConstrainAlgorithm gl_widget::get_constrain_algorithm() const
{
    return constrain_algorithm_;
}

boost::optional<CornerRef> gl_widget::pick_vertex(float mouse_x, float mouse_y) const
{
    Group*   group      = GetRootGroup(document_);
    Mat4     model      = modelview_matrix();
    Mat4     projection = projection_matrix();
    Viewport viewport   = {0, 0, width(), height()};
    Vec2     mouse      = {mouse_x, mouse_y};

    CornerRef corner_ref;
    if (PickGroupVertex(group, &model, &projection, &viewport, &mouse, &corner_ref))
    {
        Vec3 camera_position;
        GetCameraPosition(camera_.get(), &camera_position);
        Vec3 vertex_position = CornerRefPosition(&corner_ref);
        Ray camera_ray;
        RayFromPointToPoint(&camera_ray, &camera_position, &vertex_position);
        Pick pick = PickGroupBlock(group, &camera_ray);

        if (pick.block)
        {
            if (Vec3Equals(&pick.triangle.A, &vertex_position) ||
                Vec3Equals(&pick.triangle.B, &vertex_position) ||
                Vec3Equals(&pick.triangle.C, &vertex_position))
            {
                return corner_ref;
            }

            if (Vec3Distance(&camera_position, &vertex_position) >
                Vec3Distance(&camera_position, &pick.intersection))
            {
                return {};
            }
        }
        else
        {
            return corner_ref;
        }
    }

    return {};
}

void gl_widget::use_new_block_tool()
{
    use_tool<new_block_tool>();
}

void gl_widget::use_move_block_tool()
{
    use_tool<move_block_tool>();
}

void gl_widget::use_move_vertex_tool()
{
    use_tool<move_vertex_tool>();
}

void gl_widget::use_xy_plane_constraint()
{
    constrain_algorithm_ = ConstrainAlgorithm::CONSTRAIN_TO_XY_PLANE;
}

void gl_widget::use_z_axis_constraint()
{
    constrain_algorithm_ = ConstrainAlgorithm::CONSTRAIN_TO_Z_AXIS;
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
    Mat4 matrix = projection_matrix();
    glLoadMatrixf(&matrix.X.X);
}

void gl_widget::paintGL()
{
    glMatrixMode(GL_MODELVIEW);
    auto matrix = modelview_matrix();
    glLoadMatrixf(&matrix.X.X);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    draw(grid_);
    DrawGroup(GetRootGroup(document_));

    if (tool_)
    {
        tool_->paint_gl();
    }
}

Mat4 gl_widget::projection_matrix() const
{
    Mat4 axis_adjust;
    Mat4RotationX(&axis_adjust, -M_TAU_4);

    Mat4 perspective;
    Mat4Perspective(&perspective, 60.0f, float(width()) / float(height()), 0.1f, 1000.0f);

    Mat4 proj;
    Mat4Transform(&proj, &axis_adjust, &perspective);
    return proj;
}

Mat4 gl_widget::modelview_matrix() const
{
    Mat4 transform, inverse;

    CameraWorldTransform(camera_.get(), &transform);
    Mat4Inverse(&inverse, &transform);
    return inverse;
}

float minor_size(QWidget const& widget)
{
    return std::min(widget.width(), widget.height());
}

} // namespace kloss
} // namespace creator
