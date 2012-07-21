#pragma once

#include <QGLWidget>

namespace kloss {
namespace creator {

class GLWidget : public QGLWidget
{
public:
    GLWidget(QWidget* parent = nullptr);

private:
    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;
};

} // namespcae creator
} // namespace kloss
