#pragma once

#include <kloss/group.hpp>
#include <boost/optional.hpp>
#include <kloss/corner_ref.hpp>
#include <functional>

/**
 * Reference to a block and the group that the block is part of.
 * This is needed because blocks doesn't have a pointer their group.
 */
struct block_ref
{
    block_ptr block;
    group_ptr group;
};

struct pick
{
    struct block_ref block_ref;
    struct triangle triangle;
    vec3 intersection;
};

struct viewport
{
    int x;
    int y;
    int width;
    int height;
};

using for_each_block_function = std::function<void(block_ref&,vec3 const&)>;

pick pick_block(group_ptr const& group, ray const& ray);
boost::optional<corner_ref> pick_corner(group_ptr const& group, mat4 const& model, mat4 const& projection, viewport const& viewport, vec2 const& mouse);

/**
 * Sets the group position but shifts blocks and groups so that they remain
 * in the same world space position.
 */
void move_origin(group_ptr const& group, vec3 const& position);

/**
 * Traverses all blocks in the hierarchy of groups and calls the given function.
 */
void for_each_block(group_ptr const& group, vec3 const& position, for_each_block_function function);

/**
 * Returns a pointer to the child group that contains the descendant or
 * that is the descendant itself. Returns nullptr if descendant is not a
 * descendant of group.
 */
group_ptr get_child_by_descendant(group_ptr const& group, group_ptr const& descendant);
