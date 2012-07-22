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

class GLWidget : public QGLWidget
{
public:
    GLWidget(QWidget* parent = nullptr);

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
    boost::optional<cml::vector3f> cursorPosition() const;
    void drawCursor() const;

    world world_;
    camera camera_;
    vertex_array grid_;
    vertex_array cursor_;
    key_pair backwardForward_;
    key_pair leftRight_;
    QBasicTimer timer_;
    boost::optional<QPointF> mouseOrigin_;
};

/**
 * Return the smaller value of the window's width or height.
 */
float minorSize(QWidget const& widget);

} // namespcae creator
} // namespace kloss
