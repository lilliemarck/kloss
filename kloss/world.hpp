#pragma once

#include <vector>
#include <cml/cml.h>
#include <kloss/block.hpp>

namespace kloss {

class world
{
public:
    void insert(block const& block);
    void draw();

private:
    std::vector<cml::vector3f> vertices_;
    std::vector<unsigned> indices_;
};

} // namespace kloss
