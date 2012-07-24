#pragma once

#include <cml/cml.h>
#include <kloss/block.hpp>
#include <klosscreator/tool.hpp>

namespace kloss {

struct pick;

namespace creator {

class move_block_tool : public tool
{
public:
    move_block_tool(gl_widget& parent);

    void mouse_press_event(QMouseEvent const& event) override;
    void mouse_release_event(QMouseEvent const& event) override;
    void mouse_move_event(QMouseEvent const& event) override;
    void paint_gl() override;

private:
    void single_select(pick const& pick);
    void multi_select(pick const& pick);

    struct drag
    {
        drag(cml::vector3f const& origin, std::vector<block_ptr> const& blocks);
        void restore_blocks(std::vector<block_ptr> const& blocks);

        cml::vector3f reference;
        std::vector<block> original_blocks;
    };

    gl_widget& parent_;
    std::vector<block_ptr> selection_;
    boost::optional<drag> drag_;
};

} // namespace creator
} // namespace kloss
