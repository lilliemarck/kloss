#pragma once

#include <QGLWidget>

#include <kloss/camera.hpp>

namespace kloss {
namespace creator {

class GLWidget : public QGLWidget
{
public:
    GLWidget(QWidget* parent = nullptr);

private:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;

    camera camera_;
    QPointF mouseOrigin_;
};

/**
 * Return the smaller value of the window's width or height.
 */
float minorSize(QWidget const& widget);

} // namespcae creator
} // namespace kloss
