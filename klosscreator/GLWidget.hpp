#pragma once

#include <QGLWidget>
#include <QBasicTimer>
#include <kloss/camera.hpp>
#include <kloss/key_pair.hpp>
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
    void mouseMoveEvent(QMouseEvent* event) override;
    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;
    void drawCursor() const;

    camera camera_;
    vertex_array grid_;
    vertex_array cursor_;
    key_pair backwardForward_;
    key_pair leftRight_;
    QBasicTimer timer_;
    QPointF mouseOrigin_;
};

/**
 * Return the smaller value of the window's width or height.
 */
float minorSize(QWidget const& widget);

} // namespcae creator
} // namespace kloss
