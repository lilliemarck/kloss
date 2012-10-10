#pragma once

#include <kloss/geometry.hpp>
#include <kloss/math.hpp>
#include <memory>
#include <vector>

struct corner
{
    float x;
    float y;
    float top;
    float bottom;
};

struct block
{
    block();

    corner corners[4];
};

using block_ptr = std::shared_ptr<block>;

vec3 top(corner const& corner);
vec3 bottom(corner const& corner);
void translate(corner& corner, vec3 const& units);

void translate(block& block, vec3 const& units);
void get_triangles(block const& block, std::vector<triangle>& triangles);
