#pragma once

#include <memory>
#include <vector>
#include <cml/cml.h>
#include <kloss/block.hpp>

namespace kloss {

class world
{
public:
    void insert(block_ptr const& block);
    block_ptr pick(ray const& ray) const;
    void draw();

private:
    void setup_light();

    struct vertex
    {
        cml::vector3f normal;
        cml::vector3f position;
    };

    std::vector<block_ptr> blocks_;
    std::vector<vertex> vertices_;
};

} // namespace kloss
