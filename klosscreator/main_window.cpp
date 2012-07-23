#include "main_window.hpp"
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>

namespace kloss {
namespace creator {

main_window::main_window()
{
    setCentralWidget(&gl_widget_);
    gl_widget_.setFocus(Qt::OtherFocusReason);

    create_actions();
    create_menus();
    create_toolbar();
}

void main_window::create_actions()
{
    quit_action_ = new QAction(QIcon::fromTheme("application-exit"), tr("Q&uit"), this);
    quit_action_->setShortcuts(QKeySequence::Quit);
    connect(quit_action_, SIGNAL(triggered()), this, SLOT(close()));

    new_block_action_ = new QAction(tr("New Block"), this);
    new_block_action_->setCheckable(true);
    connect(new_block_action_, SIGNAL(triggered()), &gl_widget_, SLOT(use_new_block_tool()));

    move_block_action_ = new QAction(tr("Move Block"), this);
    move_block_action_->setCheckable(true);
    connect(move_block_action_, SIGNAL(triggered()), &gl_widget_, SLOT(use_move_block_tool()));

    QActionGroup* toolGroup = new QActionGroup(this);
    toolGroup->addAction(new_block_action_);
    toolGroup->addAction(move_block_action_);
    new_block_action_->trigger();
}

void main_window::create_menus()
{
    file_menu_ = menuBar()->addMenu(tr("&File"));
    file_menu_->addAction(quit_action_);
}

void main_window::create_toolbar()
{
    tool_bar_ = addToolBar(tr("Tools"));
    tool_bar_->addAction(new_block_action_);
    tool_bar_->addAction(move_block_action_);
}

} // namespace kloss
} // namespace creator
