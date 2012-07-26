#pragma once

#include <memory>
#include <vector>
#include <cml/cml.h>
#include <kloss/block.hpp>

namespace kloss {

struct viewport;

struct pick
{
    block_ptr block;
    kloss::triangle triangle;
    cml::vector3f intersection;
};

class world
{
public:
    void insert(block_ptr const& block);
    pick pick(ray const& ray) const;
    boost::optional<cml::vector3f> pick_vertex(cml::matrix44f_c const& model,
                                               cml::matrix44f_c const& projection,
                                               viewport const& viewport,
                                               cml::vector2f const& mouse) const;
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
