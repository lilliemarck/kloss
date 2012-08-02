#include "main_window.hpp"
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>

namespace kloss {
namespace creator {

main_window::main_window() :
    gl_widget_(document_)
{
    setCentralWidget(&gl_widget_);
    gl_widget_.setFocus(Qt::OtherFocusReason);

    create_actions();
    create_menus();
    create_toolbar();
}

void main_window::cut()
{
    document_.cut();
    gl_widget_.update();
}

void main_window::copy()
{
    document_.copy();
}

void main_window::paste()
{
    document_.paste();
    gl_widget_.update();
}

void main_window::del()
{
    document_.del();
    gl_widget_.update();
}

void main_window::group()
{
    document_.group_selection();
    gl_widget_.update();
}

void main_window::ungroup()
{
}

void main_window::create_actions()
{
    quit_action_ = new QAction(QIcon::fromTheme("application-exit"), tr("Q&uit"), this);
    quit_action_->setShortcuts(QKeySequence::Quit);
    connect(quit_action_, SIGNAL(triggered()), this, SLOT(close()));

    cut_action_ = new QAction(tr("Cu&t"), this);
    cut_action_->setShortcuts(QKeySequence::Cut);
    connect(cut_action_, SIGNAL(triggered()), this, SLOT(cut()));

    copy_action_ = new QAction(tr("&Copy"), this);
    copy_action_->setShortcuts(QKeySequence::Copy);
    connect(copy_action_, SIGNAL(triggered()), this, SLOT(copy()));

    paste_action_ = new QAction(tr("&Paste"), this);
    paste_action_->setShortcuts(QKeySequence::Paste);
    connect(paste_action_, SIGNAL(triggered()), this, SLOT(paste()));

    del_action_ = new QAction(tr("Delete"), this);
    del_action_->setShortcuts(QKeySequence::Delete);
    connect(del_action_, SIGNAL(triggered()), this, SLOT(del()));

    group_action_ = new QAction(tr("&Group"), this);
    group_action_->setShortcut(tr("Ctrl+G"));
    connect(group_action_, SIGNAL(triggered()), this, SLOT(group()));

    ungroup_action_ = new QAction(tr("&Ungroup"), this);
    ungroup_action_->setShortcut(tr("Ctrl+Alt+G"));
    connect(ungroup_action_, SIGNAL(triggered()), this, SLOT(ungroup()));

    new_block_action_ = new QAction(tr("New Block"), this);
    new_block_action_->setCheckable(true);
    connect(new_block_action_, SIGNAL(triggered()), &gl_widget_, SLOT(use_new_block_tool()));

    move_block_action_ = new QAction(tr("Move Block"), this);
    move_block_action_->setCheckable(true);
    connect(move_block_action_, SIGNAL(triggered()), &gl_widget_, SLOT(use_move_block_tool()));

    move_vertex_action_ = new QAction(tr("Move Vertex"), this);
    move_vertex_action_->setCheckable(true);
    connect(move_vertex_action_, SIGNAL(triggered()), &gl_widget_, SLOT(use_move_vertex_tool()));

    QActionGroup* toolGroup = new QActionGroup(this);
    toolGroup->addAction(new_block_action_);
    toolGroup->addAction(move_block_action_);
    toolGroup->addAction(move_vertex_action_);
    new_block_action_->trigger();

    xy_plane_constraint_action_ = new QAction(tr("XY"), this);
    xy_plane_constraint_action_->setCheckable(true);
    xy_plane_constraint_action_->setShortcut(Qt::Key_X);
    connect(xy_plane_constraint_action_, SIGNAL(triggered()), &gl_widget_, SLOT(use_xy_plane_constraint()));

    z_azis_constraint_action_ = new QAction(tr("Z"), this);
    z_azis_constraint_action_->setCheckable(true);
    z_azis_constraint_action_->setShortcut(Qt::Key_Z);
    connect(z_azis_constraint_action_, SIGNAL(triggered()), &gl_widget_, SLOT(use_z_axis_constraint()));

    QActionGroup* constraintGroup = new QActionGroup(this);
    constraintGroup->addAction(xy_plane_constraint_action_);
    constraintGroup->addAction(z_azis_constraint_action_);
    xy_plane_constraint_action_->trigger();
}

void main_window::create_menus()
{
    file_menu_ = menuBar()->addMenu(tr("&File"));
    file_menu_->addAction(quit_action_);

    edit_menu_ = menuBar()->addMenu(tr("&Edit"));
    edit_menu_->addAction(cut_action_);
    edit_menu_->addAction(copy_action_);
    edit_menu_->addAction(paste_action_);
    edit_menu_->addAction(del_action_);
    edit_menu_->addSeparator();
    edit_menu_->addAction(group_action_);
    edit_menu_->addAction(ungroup_action_);
}

void main_window::create_toolbar()
{
    tool_bar_ = addToolBar(tr("Tools"));
    tool_bar_->addAction(new_block_action_);
    tool_bar_->addAction(move_block_action_);
    tool_bar_->addAction(move_vertex_action_);
    tool_bar_->addSeparator();
    tool_bar_->addAction(xy_plane_constraint_action_);
    tool_bar_->addAction(z_azis_constraint_action_);
}

} // namespace kloss
} // namespace creator
