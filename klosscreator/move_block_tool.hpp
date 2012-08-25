#pragma once

#include <kloss/block.hpp>
#include <klosscreator/block_selection.hpp>
#include <klosscreator/document_lock.hpp>
#include <klosscreator/tool.hpp>

namespace kloss {

struct pick;

namespace creator {

class document;

class move_block_tool : public tool
{
public:
    move_block_tool(gl_widget& parent);
    ~move_block_tool();

    void mouse_press_event(QMouseEvent const& event) override;
    void mouse_release_event(QMouseEvent const& event) override;
    void mouse_move_event(QMouseEvent const& event) override;
    void paint_gl() override;

private:
    gl_widget& parent_;
    document& document_;
    boost::optional<Vec3> reference_;
    boost::optional<block_selection::backup_type> drag_;
    document_lock document_lock_;
};

} // namespace creator
} // namespace kloss
