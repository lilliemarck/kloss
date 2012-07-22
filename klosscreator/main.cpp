#include <QApplication>
#include <klosscreator/main_window.hpp>

using namespace kloss::creator;

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    main_window main_window;
    main_window.resize(800, 600);
    main_window.show();

    return app.exec();
}
