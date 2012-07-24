#pragma once

#include <QMainWindow>
#include <klosscreator/gl_widget.hpp>

namespace kloss {
namespace creator {

class main_window : public QMainWindow
{
    Q_OBJECT

public:
    main_window();

private:
    void create_actions();
    void create_menus();
    void create_toolbar();

    gl_widget gl_widget_;
    QAction* quit_action_;
    QAction* new_block_action_;
    QAction* move_block_action_;
    QAction* xy_plane_constraint_action_;
    QAction* z_azis_constraint_action_;
    QMenu* file_menu_;
    QToolBar* tool_bar_;
};

} // namespcae creator
} // namespace kloss
