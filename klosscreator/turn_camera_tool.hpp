#pragma once

#include <QPoint>
#include <boost/optional.hpp>

QT_BEGIN_NAMESPACE
class QMouseEvent;
QT_END_NAMESPACE

namespace kloss {
namespace creator {

class gl_widget;

class turn_camera_tool
{
public:
    turn_camera_tool(gl_widget& parent);

    void mouse_press_event(QMouseEvent const& event);
    void mouse_release_event(QMouseEvent const& event);
    void mouse_move_event(QMouseEvent const& event);

private:
    gl_widget& parent_;
    boost::optional<QPoint> mouse_origin_;
};

} // namespace creator
} // namespace kloss
