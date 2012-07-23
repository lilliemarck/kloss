#pragma once

#include <memory>
#include <vector>
#include <cml/cml.h>
#include <kloss/block.hpp>

namespace kloss {

struct pick
{
    block_ptr block;
    cml::vector3f intersection;
};

class world
{
public:
    void insert(block_ptr const& block);
    pick pick(ray const& ray) const;
    void draw();
    void update_vertex_array();

private:
    void append_vertices(block const& block);
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
