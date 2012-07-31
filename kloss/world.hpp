#pragma once

#include <memory>
#include <vector>
#include <cml/cml.h>
#include <kloss/block.hpp>
#include <kloss/corner_ref.hpp>

namespace kloss {

struct viewport;

struct pick
{
    block_ptr block;
    kloss::triangle triangle;
    cml::vector3f intersection;
};

struct viewport
{
    int x;
    int y;
    int width;
    int height;
};

class world
{
public:
    void insert(block_ptr const& block);
    pick const pick_block(ray const& ray) const;
    boost::optional<corner_ref> const pick_vertex(cml::matrix44f_c const& model,
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
