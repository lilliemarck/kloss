#pragma once

#include <QBasicTimer>
#include <QObject>
#include <kloss/key_pair.hpp>

QT_BEGIN_NAMESPACE
class QKeyEvent;
QT_END_NAMESPACE

namespace kloss {
namespace creator {

class gl_widget;

class move_camera_tool : private QObject
{
public:
    move_camera_tool(gl_widget& parent);

    void key_press_event(QKeyEvent const& event);
    void key_release_event(QKeyEvent const& event);

private:
    void timerEvent(QTimerEvent* event) override;

    gl_widget& parent_;
    key_pair backward_forward_;
    key_pair left_right_;
    QBasicTimer timer_;
};

} // namespace creator
} // namespace kloss
