#pragma once

#include <QMainWindow>
#include <klosscreator/document.hpp>
#include <klosscreator/gl_widget.hpp>

namespace kloss {
namespace creator {

class main_window : public QMainWindow
{
    Q_OBJECT

public:
    main_window();

private slots:
    void cut();
    void copy();
    void paste();
    void del();
    void group();
    void ungroup();

private:
    void create_actions();
    void create_menus();
    void create_toolbar();

    document document_;
    gl_widget gl_widget_;
    QAction* quit_action_;
    QAction* cut_action_;
    QAction* copy_action_;
    QAction* paste_action_;
    QAction* del_action_;
    QAction* group_action_;
    QAction* ungroup_action_;
    QAction* new_block_action_;
    QAction* move_block_action_;
    QAction* move_vertex_action_;
    QAction* xy_plane_constraint_action_;
    QAction* z_azis_constraint_action_;
    QMenu* file_menu_;
    QMenu* edit_menu_;
    QToolBar* tool_bar_;
};

} // namespace creator
} // namespace kloss
