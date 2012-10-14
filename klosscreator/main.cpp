#include <klosscreator/main_window.hpp>
#include <ui/application.hpp>
#include <ui/ui.h>

int main(int argc, char* argv[])
{
    ui::application app(argc, argv);
    main_window window;
    window.show();
    app.run();

    return 0;
}
