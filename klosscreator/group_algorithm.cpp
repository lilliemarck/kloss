#include "group_algorithm.hpp"
#include <limits>

struct block_pick_data
{
    float distance;
    group_ptr group;
    block_ptr block;
    struct triangle triangle;
};

static void find_nearest_block(group_ptr const& group, const ray& ray, block_pick_data& data)
{
    struct ray groupspaceray = {ray.origin - group->get_position(), ray.direction};

    std::vector<triangle> triangles;

    std::for_each(group->begin_blocks(), group->end_blocks(), [&](block_ptr& block)
    {
        get_triangles(*block, triangles);

        for (auto const& triangle : triangles)
        {
            if (auto temp = intersect_triangle(groupspaceray, triangle))
            {
                /*
                 * Use <= for distance comparsion here so that when there are
                 * multiple candidates for selection the last block added will
                 * be selected. It is useful after pasting that the new blocks
                 * can be dragged to a new location.
                 */
                if (*temp <= data.distance)
                {
                    data.distance = *temp;
                    data.group    = group;
                    data.block    = block;
                    data.triangle = triangle;
                }
            }
        }

        triangles.clear();
    });

    std::for_each(group->begin_groups(), group->end_groups(), [&](group_ptr& child)
    {
        find_nearest_block(child, ray, data);
    });
}

pick pick_block(group_ptr const& group, ray const& ray)
{
    block_pick_data data = {std::numeric_limits<float>::max()};

    find_nearest_block(group, ray, data);

    vec3 intersection = ray.origin + data.distance * ray.direction;
    return {{data.block, data.group}, data.triangle, intersection};
}

static boost::optional<vec3> project(vec3 const& in, mat4 const& model, mat4 const& projection, viewport const& viewport)
{
    mat4 transformation = projection * model;
    vec3 out = transformation * in;

    float w = transformation.x.w * in.x +
              transformation.y.w * in.y +
              transformation.z.w * in.z +
              transformation.t.w;

    if (out.z < -w || out.z > w)
    {
        return {};
    }

    out.x /= w;
    out.y /= w;
    out.z /= w;

    out.x = out.x * 0.5f + 0.5f;
    out.y = out.y * 0.5f + 0.5f;
    out.x = out.x * viewport.width + viewport.x;
    out.y = out.y * viewport.height + viewport.y;

    return out;
}

struct corner_pick_data
{
    mat4 modelview;
    mat4 projection;
    struct viewport viewport;
    vec2 mouse;
    float distance;
    corner_ref ref;
};

static void check_vertex(corner_pick_data& data, vec3 const& position, corner_ref const& ref, std::uint8_t flag)
{
    float const radius = 5.0f;

    if (auto screen_pos = project(position, data.modelview, data.projection, data.viewport))
    {
        vec2 screen_pos_xy = {screen_pos->x, data.viewport.height - screen_pos->y};
        float distance = ::distance(data.mouse, screen_pos_xy);

        if (distance < radius && screen_pos->z < data.distance)
        {
            data.distance = screen_pos->z;
            data.ref = {ref, flag};
        }
    }
}

boost::optional<corner_ref> pick_corner(group_ptr const& group, mat4 const& model, mat4 const& projection, viewport const& viewport, vec2 const& mouse)
{
    corner_pick_data data =
    {
        model,
        projection,
        viewport,
        mouse,
        std::numeric_limits<float>::max()
    };

    std::for_each(group->begin_blocks(), group->end_blocks(), [&](block_ptr const& block)
    {
        for (auto const& ref : make_corner_refs(*block))
        {
            check_vertex(data, top   (*ref), ref, corner_ref::top);
            check_vertex(data, bottom(*ref), ref, corner_ref::bottom);
        }
    });

    if (data.distance != std::numeric_limits<float>::max())
    {
        return data.ref;
    }
    else
    {
        return {};
    }
}

void move_origin(group_ptr const& group, vec3 const& position)
{
    vec3 translation = group->get_position() - position;

    std::for_each(group->begin_blocks(), group->end_blocks(), [&](block_ptr const& block)
    {
        translate(*block, translation);
    });

    std::for_each(group->begin_groups(), group->end_groups(), [&](group_ptr const& child)
    {
        child->set_position(child->get_position() + translation);
    });

    group->set_position(position);
    group->update_vertex_array();
}

void for_each_block(group_ptr const& group, vec3 const& position, for_each_block_function function)
{
    vec3 translation = position + group->get_position();

    std::for_each(group->begin_blocks(), group->end_blocks(), [&](block_ptr const& block)
    {
        block_ref ref = {block, group};
        function(ref, translation);
    });

    std::for_each(group->begin_groups(), group->end_groups(), [&](group_ptr const& child)
    {
        for_each_block(child, translation, function);
    });
}

group_ptr get_child_by_descendant(group_ptr const& group, group_ptr const& descendant)
{
    auto begin = group->begin_groups();
    auto end   = group->end_groups();

    for (auto it = begin; it != end; ++it)
    {
        if (*it == descendant)
        {
            return *it;
        }

        if (get_child_by_descendant(*it, descendant))
        {
            return *it;
        }
    };

    return nullptr;
}
