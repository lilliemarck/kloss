#include "main_window.hpp"
#include <QAction>
#include <QMenu>
#include <QMenuBar>

namespace kloss {
namespace creator {

main_window::main_window()
{
    setCentralWidget(&gl_widget_);
    gl_widget_.setFocus(Qt::OtherFocusReason);

    create_actions();
    create_menus();
}

void main_window::create_actions()
{
    quit_action_ = new QAction(QIcon::fromTheme("application-exit"), tr("Q&uit"), this);
    quit_action_->setShortcuts(QKeySequence::Quit);
    connect(quit_action_, SIGNAL(triggered()), this, SLOT(close()));
}

void main_window::create_menus()
{
    file_menu_ = menuBar()->addMenu(tr("&File"));
    file_menu_->addAction(quit_action_);
}

} // namespace kloss
} // namespace creator
