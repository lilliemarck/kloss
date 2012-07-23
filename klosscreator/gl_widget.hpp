#pragma once

#include <boost/optional.hpp>
#include <QGLWidget>
#include <QBasicTimer>
#include <kloss/camera.hpp>
#include <kloss/key_pair.hpp>
#include <kloss/world.hpp>
#include <klosscreator/vertex_array.hpp>

namespace kloss {
namespace creator {

class gl_widget : public QGLWidget
{
public:
    gl_widget(QWidget* parent = nullptr);

private:
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void timerEvent(QTimerEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;
    boost::optional<cml::vector3f> cursor_position() const;
    void draw_cursor() const;

    world world_;
    camera camera_;
    vertex_array grid_;
    vertex_array cursor_;
    key_pair backward_forward_;
    key_pair left_right_;
    QBasicTimer timer_;
    boost::optional<QPointF> mouse_origin_;
};

/**
 * Return the smaller value of the window's width or height.
 */
float minor_size(QWidget const& widget);

} // namespcae creator
} // namespace kloss
