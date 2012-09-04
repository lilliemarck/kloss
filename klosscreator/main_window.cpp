#include "main_window.hpp"
#include <ui/ui.h>

namespace kloss {
namespace creator {

main_window::main_window()
    : document_(CreateDocument())
    , window_(ui_create_window())
    , gl_widget_(new gl_widget(document_))
{
    ui_resize_window(window_, 800, 600);
    ui_set_center_widget(window_, gl_widget_->glwidget());
    ui_focus_widget(gl_widget_->glwidget());

    create_actions();
    create_menus();
    create_toolbar();
}

main_window::~main_window()
{
    ui_destroy_window(window_);
    DestroyDocument(document_);
}

void main_window::show()
{
    ui_show_window(window_);
}

void main_window::close(void* data)
{
    main_window* self = static_cast<main_window*>(data);
    ui_close_window(self->window_);
}

void main_window::cut(void* data)
{
    main_window* self = static_cast<main_window*>(data);
    CopySelectedBlocks(self->document_);
    DeleteSelectedBlocks(self->document_);
    ui_update_widget(self->gl_widget_->glwidget());
}

void main_window::copy(void* data)
{
    main_window* self = static_cast<main_window*>(data);
    CopySelectedBlocks(self->document_);
}

void main_window::paste(void* data)
{
    main_window* self = static_cast<main_window*>(data);
    PasteSelectedBlocks(self->document_);
    ui_update_widget(self->gl_widget_->glwidget());
}

void main_window::del(void* data)
{
    main_window* self = static_cast<main_window*>(data);
    DeleteSelectedBlocks(self->document_);
    ui_update_widget(self->gl_widget_->glwidget());
}

void main_window::group(void* data)
{
    main_window* self = static_cast<main_window*>(data);
    GroupSelectedBlocks(self->document_);
    ui_update_widget(self->gl_widget_->glwidget());
}

void main_window::ungroup(void* data)
{
}

static void use_new_block_tool(void* data)
{
    static_cast<gl_widget*>(data)->use_new_block_tool();
}

static void use_move_block_tool(void* data)
{
    static_cast<gl_widget*>(data)->use_move_block_tool();
}

static void use_move_vertex_tool(void* data)
{
    static_cast<gl_widget*>(data)->use_move_vertex_tool();
}

static void use_xy_plane_constraint(void* data)
{
    static_cast<gl_widget*>(data)->use_xy_plane_constraint();
}

static void use_z_axis_constraint(void *data)
{
    static_cast<gl_widget*>(data)->use_z_axis_constraint();
}

void main_window::create_actions()
{
    quit_action_ = ui_create_action(window_);
    ui_set_action_text(quit_action_, "Q&uit");
    ui_set_action_shortcut(quit_action_, "Ctrl+Q");
    ui_set_action_proc(quit_action_, &main_window::close, this);

    cut_action_ = ui_create_action(window_);
    ui_set_action_text(cut_action_, "Cu&t");
    ui_set_action_shortcut(cut_action_, "Ctrl+X");
    ui_set_action_proc(cut_action_, &main_window::cut, this);

    copy_action_ = ui_create_action(window_);
    ui_set_action_text(copy_action_, "&Copy");
    ui_set_action_shortcut(copy_action_, "Ctrl+C");
    ui_set_action_proc(copy_action_, &main_window::copy, this);

    paste_action_ = ui_create_action(window_);
    ui_set_action_text(paste_action_, "&Paste");
    ui_set_action_shortcut(paste_action_, "Ctrl+V");
    ui_set_action_proc(paste_action_, &main_window::paste, this);

    del_action_ = ui_create_action(window_);
    ui_set_action_text(del_action_, "Delete");
    ui_set_action_shortcut(del_action_, "Delete");
    ui_set_action_proc(del_action_, &main_window::del, this);

    group_action_ = ui_create_action(window_);
    ui_set_action_text(group_action_, "&Group");
    ui_set_action_shortcut(group_action_, "Ctrl+G");
    ui_set_action_proc(group_action_, &main_window::group, this);

    ungroup_action_ = ui_create_action(window_);
    ui_set_action_text(ungroup_action_, "&Ungroup");
    ui_set_action_shortcut(ungroup_action_, "Ctrl+Alt+G");
    ui_set_action_proc(ungroup_action_, &main_window::ungroup, this);

    new_block_action_ = ui_create_action(window_);
    ui_set_action_text(new_block_action_, "New Block");
    ui_set_action_checkable(new_block_action_, true);
    ui_set_action_proc(new_block_action_, use_new_block_tool, gl_widget_);

    move_block_action_ = ui_create_action(window_);
    ui_set_action_text(move_block_action_, "Move Block");
    ui_set_action_checkable(move_block_action_, true);
    ui_set_action_proc(move_block_action_, use_move_block_tool, gl_widget_);

    move_vertex_action_ = ui_create_action(window_);
    ui_set_action_text(move_vertex_action_, "Move Vertex");
    ui_set_action_checkable(move_vertex_action_, true);
    ui_set_action_proc(move_vertex_action_, use_move_vertex_tool, gl_widget_);

    ui_actiongroup *toolGroup = ui_create_actiongroup(this->window_);
    ui_add_action_to_group(toolGroup, new_block_action_);
    ui_add_action_to_group(toolGroup, move_block_action_);
    ui_add_action_to_group(toolGroup, move_vertex_action_);
    ui_trig_action(new_block_action_);

    xy_plane_constraint_action_ = ui_create_action(window_);
    ui_set_action_text(xy_plane_constraint_action_, "XY");
    ui_set_action_checkable(xy_plane_constraint_action_, true);
    ui_set_action_shortcut(xy_plane_constraint_action_, "X");
    ui_set_action_proc(xy_plane_constraint_action_, use_xy_plane_constraint, gl_widget_);

    z_azis_constraint_action_ = ui_create_action(window_);
    ui_set_action_text(z_azis_constraint_action_, "Z");
    ui_set_action_checkable(z_azis_constraint_action_, true);
    ui_set_action_shortcut(z_azis_constraint_action_, "Z");
    ui_set_action_proc(z_azis_constraint_action_, use_z_axis_constraint, gl_widget_);

    ui_actiongroup *constraintGroup = ui_create_actiongroup(this->window_);
    ui_add_action_to_group(constraintGroup, xy_plane_constraint_action_);
    ui_add_action_to_group(constraintGroup, z_azis_constraint_action_);
    ui_trig_action(xy_plane_constraint_action_);
}

void main_window::create_menus()
{
    file_menu_ = ui_create_menu(window_);
    ui_set_menu_title(file_menu_, "&File");
    ui_add_menu_action(file_menu_, quit_action_);

    edit_menu_ = ui_create_menu(window_);
    ui_set_menu_title(edit_menu_, "&Edit");
    ui_add_menu_action(edit_menu_, cut_action_);
    ui_add_menu_action(edit_menu_, copy_action_);
    ui_add_menu_action(edit_menu_, paste_action_);
    ui_add_menu_action(edit_menu_, del_action_);
    ui_add_menu_separator(edit_menu_);
    ui_add_menu_action(edit_menu_, group_action_);
    ui_add_menu_action(edit_menu_, ungroup_action_);
}

void main_window::create_toolbar()
{
    tool_bar_ = ui_create_toolbar(window_);
    ui_add_toolbar_action(tool_bar_, new_block_action_);
    ui_add_toolbar_action(tool_bar_, move_block_action_);
    ui_add_toolbar_action(tool_bar_, move_vertex_action_);
    ui_add_toolbar_separator(tool_bar_);
    ui_add_toolbar_action(tool_bar_, xy_plane_constraint_action_);
    ui_add_toolbar_action(tool_bar_, z_azis_constraint_action_);
}

} // namespace kloss
} // namespace creator
