#include <QApplication>

#include "GLWidget.hpp"

using namespace kloss::creator;

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    GLWidget glWidget;

    glWidget.resize(800, 600);
    glWidget.show();

    return app.exec();
}
