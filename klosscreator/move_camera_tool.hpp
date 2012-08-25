#pragma once

#include <QBasicTimer>
#include <QObject>
#include <kloss/keypair.h>

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
    virtual ~move_camera_tool();

    void key_press_event(QKeyEvent const& event);
    void key_release_event(QKeyEvent const& event);

private:
    void timerEvent(QTimerEvent* event) override;

    gl_widget& parent_;
    KeyPair* backward_forward_;
    KeyPair* left_right_;
    QBasicTimer timer_;
};

} // namespace creator
} // namespace kloss
