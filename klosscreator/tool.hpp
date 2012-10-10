#pragma once

#include <ui/ui.h>

class tool
{
public:
    virtual ~tool() {};
    virtual void key_pressed(ui_key key) {}
    virtual void key_released(ui_key key) {}
    virtual void mouse_pressed(ui_mouseevent const& event) {};
    virtual void mouse_released(ui_mouseevent const& event) {};
    virtual void mouse_moved(ui_mouseevent const& event) {};
    virtual void draw() {};
};
