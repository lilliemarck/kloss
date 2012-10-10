#pragma once

#include <kloss/math.hpp>
#include <klosscreator/document.hpp>
#include <klosscreator/tool.hpp>

class document;
class main_window;

class move_block_tool : public tool
{
public:
    move_block_tool(main_window& window);
    ~move_block_tool() override;

    void mouse_pressed(ui_mouseevent const& event) override;
    void mouse_released(ui_mouseevent const& event) override;
    void mouse_moved(ui_mouseevent const& event) override;
    void draw() override;

private:
    main_window& window_;
    document& document_;
    vec3 reference_;
    std::unique_ptr<block_copy> drag_backup_;
};
