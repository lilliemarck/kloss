#pragma once

#include <boost/noncopyable.hpp>
#include <QApplication>

namespace ui {

class application : private boost::noncopyable
{
public:
    application(int& argc, char** argv);
    void run();

private:
    QApplication qapplication_;
};

} // namespace ui
