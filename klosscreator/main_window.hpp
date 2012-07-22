#pragma once

#include <QMainWindow>
#include <klosscreator/GLWidget.hpp>

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

    GLWidget gl_widget_;
    QAction* quit_action_;
    QMenu* file_menu_;
};

} // namespcae creator
} // namespace kloss
