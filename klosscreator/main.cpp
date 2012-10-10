#include <klosscreator/main_window.hpp>
#include <ui/ui.h>

int main(int argc, char* argv[])
{
    ui_app *app = ui_create_app(&argc, argv);

    {
        main_window window;
        window.show();
        ui_run_app(app);
    }

    ui_destroy_app(app);

    return 0;
}
