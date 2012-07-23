#pragma once

class QMouseEvent;

namespace kloss {
namespace creator {

class gl_widget;

class tool
{
public:
    virtual ~tool() {}
    virtual void mouse_press_event(QMouseEvent* event) {}
    virtual void mouse_release_event(QMouseEvent* event) {}
    virtual void mouse_move_event(QMouseEvent* event) {}
    virtual void paint_gl() {}
};

} // namespace creator
} // namespace kloss
