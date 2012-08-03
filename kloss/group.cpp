#include "group.hpp"
#include <cassert>
#include <limits>
#include <kloss/bounding_box.hpp>
#include <kloss/group_instance.hpp>
#include <kloss/math.hpp>
#include <GL/gl.h>

namespace kloss {
namespace {

boost::optional<cml::vector3f> project(cml::vector3f const& point,
                                       cml::matrix44f_c const& model,
                                       cml::matrix44f_c const& projection,
                                       viewport const& viewport)
{
    cml::vector4f point4 = {point[0], point[1], point[2], 1.0f};
    cml::vector4f out = projection * model * point4;

    if (out[2] < -out[3] || out[2] > out[3])
    {
        return {};
    }

    out[0] /= out[3];
    out[1] /= out[3];
    out[2] /= out[3];

    out[0] = out[0] * 0.5 + 0.5f;
    out[1] = out[1] * 0.5 + 0.5f;
    out[0] = out[0] * viewport.width + viewport.x;
    out[1] = out[1] * viewport.height + viewport.y;

    return boost::optional<cml::vector3f>({out[0], out[1], out[2]});
}

} // namespace

void group::insert(block_ptr const& block)
{
    assert(block);

    blocks_.push_back(block);
    append_vertices(*block);
}

void group::remove(block_ptr const& block)
{
    kloss::remove(blocks_, block);
}

void group::insert(group_instance&& group_instance)
{
    group_instances_.push_back(std::move(group_instance));
}

pick const group::pick_block(ray const& ray) const
{
    float nearest = std::numeric_limits<float>::max();
    block_ptr nearest_block;
    triangle nearest_triangle;

    for (auto const& block : blocks_)
    {
        auto triangles = to_triangles(*block);

        for (triangle const& triangle : triangles)
        {
            if (auto temp = intersect(ray, triangle))
            {
                /*
                 * Use <= for distance comparsion here so that when there are
                 * multiple candidates for selection the last block added will
                 * be selected. It is useful after pasting that the new blocks
                 * can be dragged to a new location.
                 */
                if (*temp <= nearest)
                {
                    nearest = *temp;
                    nearest_block = block;
                    nearest_triangle = triangle;
                }
            }
        }
    }

    return {nearest_block, nearest_triangle, ray.origin + nearest * ray.direction};
}

boost::optional<corner_ref> const group::pick_vertex(cml::matrix44f_c const& model,
                                                     cml::matrix44f_c const& projection,
                                                     viewport const& viewport,
                                                     cml::vector2f const& mouse) const
{
    float const radius = 5.0f;
    float nearest_distance = std::numeric_limits<float>::max();
    boost::optional<corner_ref> nearest_corner_ref;

    auto check_vertex = [&](cml::vector3f const& position, corner_ref const& ref, uint8_t flag)
    {
        auto screen_position = project(position, model, projection, viewport);

        if (screen_position)
        {
            (*screen_position)[1] = viewport.height - (*screen_position)[1];
            auto distance = kloss::distance(mouse, to_vector2(*screen_position));

            if (distance < radius && (*screen_position)[2] < nearest_distance)
            {
                nearest_distance = (*screen_position)[2];
                nearest_corner_ref = kloss::corner_ref(ref, flag);
            }
        }
    };

    for (auto const& block : blocks_)
    {
        auto corner_refs = to_corner_refs(block);

        for (auto const& corner_ref : corner_refs)
        {
            check_vertex(top(*corner_ref),    corner_ref, corner_ref::top_flag);
            check_vertex(bottom(*corner_ref), corner_ref, corner_ref::bottom_flag);
        }
    }

    return nearest_corner_ref;
}

namespace {

void expand(bounding_box& bbox, corner const& corner)
{
    min(bbox.lower[0], corner.x);
    max(bbox.upper[0], corner.x);
    min(bbox.lower[1], corner.y);
    max(bbox.upper[1], corner.y);
    min(bbox.lower[2], corner.bottom);
    max(bbox.upper[2], corner.top);
}

} // namespace

bounding_box group::bounding_box(cml::vector3f const& group_position) const
{
    kloss::bounding_box bbox;

    if (!blocks_.empty())
    {
        for (auto const& block : blocks_)
        {
            for (auto corner : *block)
            {
                expand(bbox, corner);
            }
        }

        // Don't translate if there are no blocks
        translate(bbox, group_position);
    }

    for (auto const& group_instance : group_instances_)
    {
        expand(bbox, group_instance.bounding_box(group_position));
    }

    return bbox;
}

void group::draw() const
{
    if (!vertices_.empty())
    {
        glNormalPointer(GL_FLOAT, sizeof(vertex), vertices_.front().normal.data());
        glVertexPointer(3, GL_FLOAT, sizeof(vertex), vertices_.front().position.data());
        glDrawArrays(GL_TRIANGLES, 0, vertices_.size());
    }

    for (auto const& group_instance : group_instances_)
    {
        group_instance.draw();
    }
}

void group::update_vertex_array()
{
    vertices_.clear();

    for (auto const& block : blocks_)
    {
        append_vertices(*block);
    }
}

void group::append_vertices(block const& block)
{
    auto triangles = to_triangles(block);

    for (auto const& triangle : triangles)
    {
        cml::vector3f normal = make_normal(triangle);
        vertices_.push_back({normal, triangle.a});
        vertices_.push_back({normal, triangle.b});
        vertices_.push_back({normal, triangle.c});
    }
}

void draw(group const& group)
{
    glPushAttrib(GL_ENABLE_BIT | GL_LIGHTING_BIT);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    cml::vector4f light_position = {-1.0f, -2.0f, 3.0f, 0.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, light_position.data());

    glEnable(GL_DEPTH_TEST);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    group.draw();

    glPopClientAttrib();
    glPopAttrib();
}

} // namespace kloss
