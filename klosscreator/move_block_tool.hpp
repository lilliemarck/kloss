#pragma once

#include <boost/optional.hpp>
#include <kloss/block.h>
#include <klosscreator/blockselection.h>
#include <klosscreator/document_lock.hpp>
#include <klosscreator/tool.hpp>

struct Document;
struct Pick;

namespace kloss {
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
    Document* document_;
    boost::optional<Vec3> reference_;
    Buffer* drag_;
    document_lock document_lock_;
};

} // namespace creator
} // namespace kloss
