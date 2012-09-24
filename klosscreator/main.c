#include <klosscreator/mainwindow.h>
#include <ui/ui.h>

int main(int argc, char* argv[])
{
    ui_app *app = ui_create_app(&argc, argv);
    mainwindow *win = create_mainwindow();
    show_mainwindow(win);
    ui_run_app(app);
    destroy_mainwindow(win);
    ui_destroy_app(app);

    return 0;
}
