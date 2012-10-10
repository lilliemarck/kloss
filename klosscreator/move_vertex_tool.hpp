#pragma once

#include <klosscreator/corner_selection.hpp>
#include <klosscreator/tool.hpp>

class document;
class main_window;

class move_vertex_tool : public tool
{
public:
    move_vertex_tool(main_window& window);
    ~move_vertex_tool() override;

    void mouse_pressed(ui_mouseevent const& event) override;
    void mouse_released(ui_mouseevent const& event) override;
    void mouse_moved(ui_mouseevent const& event) override;
    void draw() override;

private:
    main_window& window_;
    document& document_;
    corner_selection selection_;
    vec3 reference_;
    boost::optional<std::vector<corner>> drag_backup_;
};
