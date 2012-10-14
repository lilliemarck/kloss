#include "application.hpp"

namespace ui {

application::application(int& argc, char** argv) :
    qapplication_(argc, argv)
{
}

void application::run()
{
    qapplication_.exec();
}

} // namespace ui
