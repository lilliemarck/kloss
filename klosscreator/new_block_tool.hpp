#pragma once

#include <boost/optional.hpp>
#include <kloss/math.hpp>
#include <klosscreator/tool.hpp>

class main_window;

class new_block_tool : public tool
{
public:
    new_block_tool(main_window& window);

    void mouse_pressed(ui_mouseevent const& event) override;
    void mouse_moved(ui_mouseevent const& event) override;
    void draw() override;

private:
    main_window& window_;
    boost::optional<vec3> cursor_;
};
