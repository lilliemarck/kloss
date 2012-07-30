#pragma once

#include <array>
#include <vector>
#include "geometry.hpp"

namespace kloss {

struct corner
{
    float x;
    float y;
    float top;
    float bottom;
};

cml::vector3f top(corner const& corner);
cml::vector3f bottom(corner const& corner);

using block = std::array<corner,4>;
using block_ptr = std::shared_ptr<block>;

enum class block_side
{
    top,
    bottom
};

class vertex_ref
{
public:
    vertex_ref(block_ptr const& block, uint8_t corner_index, block_side side);
    bool const operator==(vertex_ref const& rhs) const;
    bool const operator!=(vertex_ref const& rhs) const;
    cml::vector3f const to_vector() const;

private:
    block_ptr block_;
    uint8_t corner_index_;
    block_side side_;
};

void translate(block& block, cml::vector3f const& units);
std::vector<triangle> to_triangles(block const& block);
std::vector<vertex_ref> to_vertex_refs(block_ptr const& block);

} // namespace kloss
