#pragma once

#include <memory>
#include <QGLWidget>
#include <boost/optional.hpp>
#include <kloss/memory.hpp>
#include <kloss/group.h>
#include <klosscreator/constrain.h>
#include <klosscreator/move_camera_tool.hpp>
#include <klosscreator/turn_camera_tool.hpp>
#include <klosscreator/vertex_array.hpp>

struct Camera;
struct Document;

namespace kloss {
namespace creator {

class tool;

class gl_widget : public QGLWidget
{
    Q_OBJECT

public:
    gl_widget(Document* document);
    ~gl_widget();

    Document* document();
    Group* group();
    Camera* camera();
    Ray mouse_ray(float mouse_x, float mouse_y) const;
    vertex_array const& cursor_vertices() const;
    ConstrainAlgorithm get_constrain_algorithm() const;
    boost::optional<CornerRef> pick_vertex(float mouse_x, float mouse_y) const;

public slots:
    void use_new_block_tool();
    void use_move_block_tool();
    void use_move_vertex_tool();
    void use_xy_plane_constraint();
    void use_z_axis_constraint();

private:
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;
    Mat4 projection_matrix() const;
    Mat4 modelview_matrix() const;

    template <typename Tool>
    void use_tool()
    {
        tool_ = make_unique<Tool>(*this);
        update();
    }

    Document* document_;
    std::shared_ptr<Camera> camera_;
    vertex_array grid_;
    vertex_array cursor_;
    ConstrainAlgorithm constrain_algorithm_;
    move_camera_tool move_camera_tool_;
    turn_camera_tool turn_camera_tool_;
    std::unique_ptr<tool> tool_;
};

/**
 * Return the smaller value of the window's width or height.
 */
float minor_size(QWidget const& widget);

} // namespace creator
} // namespace kloss
