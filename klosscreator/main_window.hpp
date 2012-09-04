#pragma once

#include <klosscreator/document.h>
#include <klosscreator/gl_widget.hpp>

struct ui_action;
struct ui_menu;
struct ui_toolbar;
struct ui_window;

namespace kloss {
namespace creator {

class main_window
{
public:
    main_window();
    ~main_window();

    void show();

    static void close(void* data);
    static void cut(void* data);
    static void copy(void* data);
    static void paste(void* data);
    static void del(void* data);
    static void group(void* data);
    static void ungroup(void* data);

private:
    void create_actions();
    void create_menus();
    void create_toolbar();

    Document* document_;
    ui_window* window_;
    gl_widget* gl_widget_;
    ui_action* quit_action_;
    ui_action* cut_action_;
    ui_action* copy_action_;
    ui_action* paste_action_;
    ui_action* del_action_;
    ui_action* group_action_;
    ui_action* ungroup_action_;
    ui_action* new_block_action_;
    ui_action* move_block_action_;
    ui_action* move_vertex_action_;
    ui_action* xy_plane_constraint_action_;
    ui_action* z_azis_constraint_action_;
    ui_menu* file_menu_;
    ui_menu* edit_menu_;
    ui_toolbar* tool_bar_;
};

} // namespace creator
} // namespace kloss
