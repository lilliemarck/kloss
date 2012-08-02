#pragma once

#include <memory>
#include <vector>
#include <cml/cml.h>
#include <kloss/block.hpp>
#include <kloss/corner_ref.hpp>
#include <kloss/group_instance.hpp>

namespace kloss {

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

class group
{
public:
    void insert(block_ptr const& block);
    void remove(block_ptr const& block);

    void insert(group_instance&& group_instance);

    pick const pick_block(ray const& ray) const;
    boost::optional<corner_ref> const pick_vertex(cml::matrix44f_c const& model,
                                                  cml::matrix44f_c const& projection,
                                                  viewport const& viewport,
                                                  cml::vector2f const& mouse) const;
    void draw() const;
    void update_vertex_array();

private:
    void append_vertices(block const& block);

    struct vertex
    {
        cml::vector3f normal;
        cml::vector3f position;
    };

    std::vector<block_ptr> blocks_;
    std::vector<group_instance> group_instances_;
    std::vector<vertex> vertices_;
};

void draw(group const& group);

} // namespace kloss
