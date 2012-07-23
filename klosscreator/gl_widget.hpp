#pragma once

#include <memory>
#include <boost/optional.hpp>
#include <QGLWidget>
#include <QBasicTimer>
#include <kloss/camera.hpp>
#include <kloss/key_pair.hpp>
#include <kloss/world.hpp>
#include <klosscreator/vertex_array.hpp>

namespace kloss {
namespace creator {

class tool;

class gl_widget : public QGLWidget
{
    Q_OBJECT

public:
    gl_widget(QWidget* parent = nullptr);
    ~gl_widget();

    world& world();
    boost::optional<cml::vector3f> cursor_position() const;

public slots:
    void use_new_block_tool();
    void use_move_block_tool();

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
    void draw_cursor() const;

    kloss::world world_;
    camera camera_;
    vertex_array grid_;
    vertex_array cursor_;
    key_pair backward_forward_;
    key_pair left_right_;
    QBasicTimer timer_;
    boost::optional<QPointF> mouse_origin_;
    std::unique_ptr<tool> tool_;
};

/**
 * Return the smaller value of the window's width or height.
 */
float minor_size(QWidget const& widget);

} // namespace creator
} // namespace kloss
